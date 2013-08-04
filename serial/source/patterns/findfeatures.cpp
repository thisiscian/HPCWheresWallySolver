#include <whereswally/patterns.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cfloat>
#include <map>
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;
using namespace wwp;

Find_Features::Find_Features() {
  info.set_name("Find Features");
  info.set_description("Locate Wally by SURF feature matching");
  info.set_confidence(0.9);
  minHessian = 100;
  knn_depth = 1;
}

// Using the SURF feature detector, find keypoitns within an image
vector<KeyPoint> Find_Features::get_keypoints_from_image(Mat image) {
  vector<KeyPoint> keypoints;
  Mat scene_descriptors;
  SurfFeatureDetector surf_detector(minHessian);

  // assume they have a specific transparency background colour
  Mat image_mask = get_colour_in_image(image, "#FF00FF", "#FF00FF", 1, 0, 1, 1, 0, 1);
  image_mask = image_mask < 125;

  surf_detector.detect(image, keypoints, image_mask);
  return keypoints;
}

// identify best quality homography
double homography_quality(homography H) {
  if(H.a <= 0 || H.c <= 0) {
    return 10;
  }
  double scale = H.a/H.c;
  double min_scale = min(scale,1/scale);
  return fabs(1-min_scale);
}

homography calculate_scale_and_translation(vector< vector<DMatch > > matches, vector<KeyPoint> object_keypoints, vector<KeyPoint> scene_keypoints, int i, int j, int k, int l) {
  int match_one_scene_id = matches[i][j].trainIdx;
  int match_one_object_id = matches[i][j].queryIdx;
  int match_two_scene_id = matches[k][l].trainIdx;
  int match_two_object_id = matches[k][l].queryIdx;

  Point2f match_one_object_point = object_keypoints[match_one_object_id].pt;
  Point2f match_two_object_point = object_keypoints[match_two_object_id].pt;

  Point2f match_one_scene_point = scene_keypoints[match_one_scene_id].pt;
  Point2f match_two_scene_point = scene_keypoints[match_two_scene_id].pt;
  
  homography H;

  H.a = (match_one_object_point.x-match_two_object_point.x)/(match_one_scene_point.x-match_two_scene_point.x);
  H.b = match_one_scene_point.x -H.a*match_one_object_point.x;
  H.c = (match_one_object_point.y-match_two_object_point.y)/(match_one_scene_point.y-match_two_scene_point.y);
  H.d = match_one_scene_point.y -H.c*match_one_object_point.y;  
  return H;
}

vector<Pattern_Result> Find_Features::start_search(Mat scene_image) {
  vector<Pattern_Result> results;
  vector <string> objects;
  SurfDescriptorExtractor surf_extractor;
  Mat scene_descriptors, object_descriptors;
  FlannBasedMatcher matcher;
  vector< vector< DMatch > > matches, good_matches;

  map< homography, vector< DMatch> > scale_and_translation_matches;
  // add all the potential objects to be searched over to the objects list
//  objects.push_back("feature_samples/wally_large_generic.png");
//  objects.push_back("feature_samples/wally_small_only_head.png");
//  objects.push_back("feature_samples/wally_from_beach_with_background.png");
  objects.push_back("feature_samples/wally_from_beach_no_background.png");
//  objects.push_back("feature_samples/wally_from_shopping_mall_with_background.png");
  objects.push_back("feature_samples/wally_from_shopping_mall_no_background.png");

  SIFT sift;
  Mat image_mask(scene_image.rows, scene_image.rows, CV_8U, Scalar(255));
  vector<KeyPoint> scene_keypoints;
  sift(scene_image, image_mask, scene_keypoints, scene_descriptors);

  // TEMPORARY OBJECTS FOR VISUALISATION
  Mat scene_clone = scene_image.clone();
  //namedWindow("matches", CV_WINDOW_NORMAL);

  for(int i=0; i<objects.size(); i++) {
    vector<homography> homography_list;
    matches.clear();
    good_matches.clear();
    cout << "=========================" << endl;
    cout << "IMAGE " << i+1 << "/" << objects.size() << ": " << objects[i] << endl;
    cout << "=========================" << endl;

    Mat object_image = imread(objects[i], CV_LOAD_IMAGE_COLOR);
    vector<KeyPoint> object_keypoints;
    image_mask = get_colour_in_image(object_image, "#FF00FF", "#FF00FF", 1, 0, 1, 1, 0, 1);
    image_mask = image_mask < 125;

    sift(object_image, image_mask, object_keypoints, object_descriptors);
    matcher.knnMatch(object_descriptors, scene_descriptors, matches, knn_depth );

    cout << "Calculating min descriptor distances: " << flush;
    double min_dist = DBL_MAX;
    int min_id;
    for(int j=0; j<object_descriptors.rows; j++) {
      double min_distance = matches[j][0].distance;
      if(min_distance < min_dist) {min_dist = min_distance; min_id = j;}
      cout << "\033[39G\033[0K" << j*100.0/(matches.size()) << "%";
    }
    cout << "\033[39G\033[0K" << 100.0 << "% (Complete)" << endl;

    min_dist  = max(min_dist, numeric_limits<float>::epsilon()+1e-05);
    cout << "Calculating good keypoint matches: ";
    float distance_multiplier=0.5;
    while( good_matches.size() <=distance_multiplier*min_dist) {
      good_matches.clear();
      distance_multiplier+=0.5;
      matcher.radiusMatch(object_descriptors, scene_descriptors, good_matches, distance_multiplier*min_dist);
      if(distance_multiplier*min_dist>10) {
        break;
      }
    }
    cout << "\033[36G\033[0K" << 100.0 << "% (Complete)" << endl;

    cout << "Creating Transform fitted Matches: " << flush;
    for(int j=0; j<matches.size(); j++) {
      for(int k=0; k<matches[j].size(); k++) {
        for(int l=j+1; l<matches.size(); l++) {
          for(int m=0; m<matches[l].size(); m++) {
            homography H = calculate_scale_and_translation(matches, object_keypoints, scene_keypoints, j,k,l,m);
            homography_list.push_back(H);
            scale_and_translation_matches[H].push_back(matches[j][k]);
            scale_and_translation_matches[H].push_back(matches[l][m]);
            cout << "\033[36G\033[0K" << (k+1+matches.size()*(j+1))*100.0/((good_matches.size()-1)*good_matches.size()) << "%";
          }
        }
      }
    }
    cout << "\033[36G\033[0K" << 100.0 << "% (Complete)" << endl;

    homography min_H;
    min_H.a = 0;
    min_H.b = 0;
    min_H.c = 0;
    min_H.d = 0;

    cout << "Analysing Homography: " << flush;
    for(int j=0; j<homography_list.size(); j++) {
      if(homography_quality(homography_list[j]) < homography_quality(min_H)) {
        min_H = homography_list[j];
      }
    }
    cout << "Homography: diff(H) = " << homography_quality(min_H) << " (" << min_H.a << "," << min_H.b << "," << min_H.c << "," << min_H.d << ")" << endl << endl;
    float content[9] = {min_H.a,0,min_H.b,0,min_H.c,min_H.d,0,0,1};
    Mat H(3,3,CV_32F, content);

    vector<Point2f> obj_corners(4), scene_corners(4);
    obj_corners[0] = cvPoint(0,0);
    obj_corners[1] = cvPoint( object_image.cols, 0 );
    obj_corners[2] = cvPoint( object_image.cols, object_image.rows );
    obj_corners[3] = cvPoint( 0, object_image.rows );

    perspectiveTransform( obj_corners, scene_corners, H);
    /*
 
    int min_col = 125;
    int num = (i+1)*255.0*(1001001)/objects.size();

    int R = 0, G = 0, B = 0;
    if(i%6 == 0 || i%6 == 3 || i%6 ==4)  { R = (255-min_col)*(1-(i*objects.size()/6.0+1)+min_col);}
    if(i%6 == 1 || i%6 == 3 || i%6 ==5)  { G = (255-min_col)*(1-(i*objects.size()/6.0+1)+min_col);}
    if(i%6 == 2 || i%6 == 4 || i%6 ==5)  { B = (255-min_col)*(1-(i*objects.size()/6.0+1)+min_col);}

    line( scene_clone, scene_corners[0], scene_corners[1], Scalar(  B, G, R ), 4 );
    line( scene_clone, scene_corners[1], scene_corners[2], Scalar(  B, G, R ), 4 );
    line( scene_clone, scene_corners[2], scene_corners[3], Scalar(  B, G, R ), 4 );
    line( scene_clone, scene_corners[3], scene_corners[0], Scalar(  B, G, R ), 4 );
    if(scene_keypoints.size() <= 0 || object_keypoints.size() <= 0 ) { continue;}
    cout << scene_keypoints.size() << " " << object_keypoints.size() << " " << scale_and_translation_matches[min_H].size() << endl;
    imshow( "matches", scene_clone );
    waitKey(0);
    */
    if(homography_quality(min_H) < 0.01) {
      Pattern_Result tmp;
      tmp.info = info;
      tmp.wally_location[0] = (scene_corners[0].x+scene_corners[1].x)/2;
      tmp.wally_location[1] = (scene_corners[0].y+scene_corners[2].y)/2;
      tmp.scale[0] = (scene_corners[2].x-scene_corners[0].x)/2;
      tmp.scale[1] = (scene_corners[2].y-scene_corners[0].y)/2;
      tmp.certainty = (1-homography_quality(min_H));
      results.push_back(tmp);
    }
  }

  return results;
}
