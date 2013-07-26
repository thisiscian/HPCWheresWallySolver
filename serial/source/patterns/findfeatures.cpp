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
  knn_depth = 10;
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

double identity_difference(Mat H) {
  Mat squared;
  float identity_values[9] = {1,0,0,0,1,0,0,0,1};
  Mat identity(3,3,CV_32F, identity_values);
  identity.convertTo(identity, H.depth());
  multiply(H,H,squared);
  squared = identity - squared;
  return fabs(sum(squared)[0]);
}

string calculate_scale_and_translation(vector< vector<DMatch > > matches, vector<KeyPoint> object_keypoints, vector<KeyPoint> scene_keypoints, int i, int j, int k, int l) {
  int match_one_scene_id = matches[i][j].trainIdx;
  int match_one_object_id = matches[i][j].queryIdx;
  int match_two_scene_id = matches[k][l].trainIdx;
  int match_two_object_id = matches[k][l].queryIdx;

  Point2f match_one_object_point = object_keypoints[match_one_object_id].pt;
  Point2f match_two_object_point = object_keypoints[match_two_object_id].pt;

  Point2f match_one_scene_point = scene_keypoints[match_one_scene_id].pt;
  Point2f match_two_scene_point = scene_keypoints[match_two_scene_id].pt;
  
  double a = floor((match_one_object_point.x-match_two_object_point.x)/(match_one_scene_point.x-match_two_scene_point.x)*2)/2;
  int b = match_one_scene_point.x -a*match_one_object_point.x;
  double c = floor((match_one_object_point.y-match_two_object_point.y)/(match_one_scene_point.y-match_two_scene_point.y)*2)/2;
  int d = match_one_scene_point.y -a*match_one_object_point.y;

  if( a > 0 && c > 0 && fabs(a-c) <= 1) {
    stringstream out;
    out << a << "," << b << "," << c << "," << d;
    return out.str();
  }
  return "NULL";
}

vector<Pattern_Result> Find_Features::start_search(Mat scene_image) {
  vector <string> objects;
  SurfDescriptorExtractor surf_extractor;
  Mat scene_descriptors, object_descriptors;
  FlannBasedMatcher matcher;
  vector< vector< DMatch > > matches, good_matches;

  map< string, vector< DMatch> > scale_and_translation_matches;

  // add all the potential objects to be searched over to the objects list
  objects.push_back("feature_samples/wally_large_generic.png");
  objects.push_back("feature_samples/wally_from_beach_with_background.png");
  objects.push_back("feature_samples/wally_from_beach_no_background.png");
  objects.push_back("feature_samples/wally_from_shopping_mall_with_background.png");
  objects.push_back("feature_samples/wally_from_shopping_mall_no_background.png");

  Mat image_mask = get_colour_in_image(scene_image, "#FF00FF", "#FF00FF", 1, 0, 1, 1, 0, 1);
  image_mask = image_mask < 125;
  vector<KeyPoint> scene_keypoints;// = get_keypoints_from_image(scene_image);
  SIFT sift;
  sift(scene_image, image_mask, scene_keypoints, scene_descriptors);
//  surf_extractor.compute(scene_image, scene_keypoints, scene_descriptors);

  // TEMPORARY OBJECTS FOR VISUALISATION
  Mat scene_clone = scene_image.clone();
  namedWindow("matches", CV_WINDOW_NORMAL);

  for(int i=0; i<objects.size(); i++) {
    matches.clear();
    good_matches.clear();
    cout << "=========================" << endl;
    cout << "IMAGE " << i+1 << "/" << objects.size() << ": " << objects[i] << endl;
    cout << "=========================" << endl;

    Mat object_image = imread(objects[i], CV_LOAD_IMAGE_COLOR);
    vector<KeyPoint> object_keypoints;// = get_keypoints_from_image(object_image);
    image_mask = get_colour_in_image(object_image, "#FF00FF", "#FF00FF", 1, 0, 1, 1, 0, 1);
    image_mask = image_mask < 125;

    sift(object_image, image_mask, object_keypoints, object_descriptors);
//    surf_extractor.compute(object_image, object_keypoints, object_descriptors);
    matcher.knnMatch(object_descriptors, scene_descriptors, matches, knn_depth );

    cout << "Calculating min descriptor distances: ";
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
    while( good_matches.size() <=4) {
      good_matches.clear();
      distance_multiplier+=0.5;
      matcher.radiusMatch(object_descriptors, scene_descriptors, good_matches, distance_multiplier*min_dist);
      if(distance_multiplier*min_dist>10) {
        break;
      }
    }
    cout << "\033[36G\033[0K" << 100.0 << "% (Complete)" << endl;


    cout << "Creating Transform fitted Matches: ";
    for(int j=0; j<good_matches.size(); j++) {
      for(int k=0; k<good_matches[j].size(); k++) {
        for(int l=j+1; l<good_matches.size(); l++) {
          for(int m=0; m<good_matches[l].size(); m++) {
            string tmp = calculate_scale_and_translation(good_matches, object_keypoints, scene_keypoints, j,k,l,m);
            if(tmp != "NULL") {
              scale_and_translation_matches[tmp].push_back(good_matches[j][k]);
              scale_and_translation_matches[tmp].push_back(good_matches[l][m]);
            }
            cout << "\033[36G\033[0K" << (k+1+good_matches.size()*(j+1))*100.0/((good_matches.size()-1)*good_matches.size()) << "%";
          }
        }
      }
    }
    cout << "\033[36G\033[0K" << 100.0 << "% (Complete)" << endl;

    vector<Point2f> obj, min_obj;
    vector<Point2f> scene, min_scene;
    Mat H, min_H(3,3,CV_32F, Scalar(100));

    string min_string;
    cout << "Analysing Homography: ";
    
    cout << "scale_and_translation_matches.size() = " << scale_and_translation_matches.size()<< endl;
    for(map<string,vector<DMatch> >::iterator it=scale_and_translation_matches.begin(); it!=scale_and_translation_matches.end(); ++it) {
      obj.clear();  
      scene.clear();
      cout << it->first << " " << it->second.size() << endl;
      if(it->second.size() < 4) {continue;}
      for( int j=0; j<it->second.size(); j++ ) {
        obj.push_back( object_keypoints[it->second[j].queryIdx ].pt );
        scene.push_back( scene_keypoints[it->second[j].trainIdx ].pt );
      }
      H = findHomography( obj, scene, CV_RANSAC );
      cout << identity_difference(H) << endl;
      if(identity_difference(H) < identity_difference(min_H)) {
        min_H = H;
        min_obj = obj;
        min_scene = scene;
        min_string = it->first;
      }
    }
    obj = min_obj;
    scene = min_scene;
    H = min_H;
    cout << "Homography: diff(H) = " << identity_difference(H) << endl << endl;

    std::vector<Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( object_image.cols, 0 );
    obj_corners[2] = cvPoint( object_image.cols, object_image.rows ); obj_corners[3] = cvPoint( 0, object_image.rows );
    std::vector<Point2f> scene_corners(4);

    perspectiveTransform( obj_corners, scene_corners, H);
    //cerr << "drawing box on scene" << endl;
 
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

    //-- Show detected matches
    imshow( "matches", scene_clone );
//    waitKey(0);
  }



  vector<Pattern_Result> results;
  return results;
}
