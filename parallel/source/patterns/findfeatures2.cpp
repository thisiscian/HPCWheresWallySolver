#include <whereswally/patterns.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <cfloat>
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

vector<KeyPoint> Find_Features::get_keypoints_from_image(Mat image) {
  SurfFeatureDetector surf_detector(minHessian);
  Mat new_image = get_colour_in_image(image, "#FF00FF", "#FF00FF", 1, 0, 1, 1, 0, 1);
  new_image = new_image < 125;
  vector<KeyPoint> image_keypoints;
  Mat image_descriptors, grey_image;
  cvtColor(image, grey_image, CV_RGB2GRAY);
  surf_detector.detect(grey_image, image_keypoints, new_image);
  return image_keypoints;
}

void print_status(int i, int I, int j, int J, string task) {
  cout << "\033[A\033[2K\033[A\033[2K" << "Comparing image against object " << i << " of " <<  I-1 << endl;
  cout << task << ": " << setw(3) << setprecision(2) << (double)(j)*100.0/(J-1) << "% complete" << endl;
}

vector<Pattern_Result> Find_Features::start_search(Mat image) {
  vector <string> feature_samples;
  feature_samples.push_back("feature_samples/wally_large_generic.png");
  feature_samples.push_back("feature_samples/wally_from_beach_with_background.png");
  feature_samples.push_back("feature_samples/wally_from_beach_no_background.png");
  feature_samples.push_back("feature_samples/wally_from_shopping_mall_with_background.png");
  feature_samples.push_back("feature_samples/wally_from_shopping_mall_no_background.png");

  vector<KeyPoint> image_keypoints = get_keypoints_from_image(image);
  SurfDescriptorExtractor surf_extractor;
  Mat image_descriptors, feature_sample_image_descriptors;
  surf_extractor.compute(image, image_keypoints, image_descriptors);

  Mat image_clone = image.clone();

  namedWindow("matches", CV_WINDOW_NORMAL);
  FlannBasedMatcher matcher;
  vector< vector< DMatch > > matches, good_matches;
  cout << "Comparing images against object" << endl;
  int gapToTop = 0;
  int permaGap = 0;
  for(int i=0; i<feature_samples.size(); i++) {
    matches.clear();
    good_matches.clear();
    gapToTop = permaGap;
    cout << "======" << endl;
    cout << "IMAGE: " << feature_samples[i] << endl;
    cout << "======" << endl;
    cout << "Comparing images against object: " << i+1 << "/" <<  feature_samples.size() << endl;

    Mat feature_sample_image = imread(feature_samples[i], CV_LOAD_IMAGE_COLOR);
    vector<KeyPoint> feature_sample_image_keypoints = get_keypoints_from_image(feature_sample_image);

    surf_extractor.compute(feature_sample_image, feature_sample_image_keypoints, feature_sample_image_descriptors);
    
    matcher.knnMatch(feature_sample_image_descriptors, image_descriptors, matches, knn_depth );

    
    cout << "Calculating max and min descriptor distances: ";
    double max_dist = 0, min_dist = 100;
    double min_id, max_id;
    for(int j=0; j<matches.size(); j++) {
      double min_distance = matches[j][0].distance;
      double max_distance = matches[j][knn_depth-1].distance;
      if(min_distance < min_dist) {min_dist = min_distance; min_id = j;}
      if(max_distance > max_dist) {max_dist = max_distance; max_id = j;}
      cout << "\033[47G\033[0K" << j*100.0/(matches.size()) << "%";
    }
    cout << "\033[47G\033[0K" << 100.0 << "% (Complete)" << endl;
    gapToTop++;

    //cerr << "min = " << min_dist << "; max = " << max_dist << endl;
    cout << "Calculating good keypoint matches: ";
    float distance_multiplier=0.5;
    while( good_matches.size() <=4) {
      good_matches.clear();
      distance_multiplier+=0.5;
      for( int j=0; j<matches.size(); j++ ) {
        vector <DMatch> tmp;
        good_matches.push_back(tmp);
        for( int k=0; k<matches[j].size(); k++) {
          if( matches[j][k].distance <= distance_multiplier*min_dist && fabs(image_keypoints[matches[j][k].trainIdx].angle-feature_sample_image_keypoints[matches[j][k].queryIdx].angle) < 10) {
            good_matches[good_matches.size()-1].push_back( matches[j][k] ); 
          }
          cout << "\033[36G\033[0K" << (j+double(k)/matches[j].size())*100.0/(matches.size()) << "%";
        }
        if(good_matches[good_matches.size()-1].size() == 0) {
          good_matches.pop_back();
        }
      }
      if(distance_multiplier>10) {
        break;
      }
    }
    cout << "\033[36G\033[0K" << 100.0 << "% (Complete)" << endl;
    gapToTop++;
/*
    good_matches.clear();
    vector < vector <DMatch> > match_two = matches;
    int angle = image_keypoints[match_two[min_id][0].trainIdx].angle;
    for(int j=0; j<match_two.size(); j++) {
      vector <DMatch> tmp;
      vector <int> min_vec;
      for(int k=0; k<match_two[j].size(); k++) {
        int trainIdx = matches[j][k].trainIdx;
        int queryIdx = matches[j][k].queryIdx;
        int angle_one = image_keypoints[trainIdx].angle;
        int angle_two = image_keypoints[queryIdx].angle;
        int minimum = INT_MAX;
        for(int l=0;l<4;l++) {
          int current_minimum = fabs(angle -(angle_one+l*90)%360);
          if(current_minimum < minimum) minimum = current_minimum;
        }
        int l=0;
        if(min_vec.size() == 0) {
          min_vec.push_back(minimum);
        } else {
          while( l<min_vec.size()-1 && minimum > min_vec[l] ) {
            l++;
          }
        }
        if( fabs(angle_one-angle_two) < 10 && minimum < 10 ) {
          tmp.insert(tmp.begin()+l, match_two[j][k]);
          min_vec.insert(min_vec.begin()+l, minimum);
        }
      }
      if(tmp.size() > 0) {
        good_matches.push_back(tmp);
      }
    }
*/

    if(good_matches.size() < 4) {
      continue;
    }
        //good_matches = matches;
  
    Mat img_matches;
    //cerr << "good matches = " << good_matches.size() << " from " << matches.size() << " (at multiplier = " << distance_multiplier << ")" << endl;
    float dist = 0;
    for(int j=0; j<good_matches.size(); j++) {
      dist += good_matches[j][0].distance;
    }
    //cerr << "match quality = " << dist/good_matches.size() << endl;
    //cerr << "Placing object on scene" << endl;
    //-- Localize the object
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;

    cout << "Analysing Homography: ";
    for( int j=0; j<good_matches.size(); j++ ) {
      obj.push_back( feature_sample_image_keypoints[ good_matches[j][0].queryIdx ].pt );
      scene.push_back( image_keypoints[ good_matches[j][0].trainIdx ].pt );
        cout << "\033[23G\033[0K" << j*100.0/(good_matches.size()) << "%";
    }
    cout << "\033[23G\033[0K" << 100.0 << "% (Complete)" << endl;
    gapToTop++;

    Mat H = findHomography( obj, scene, CV_RANSAC );
    cout << H << endl;
    cout << "Homography: det(H) = " << determinant(H) << endl << endl;
    permaGap++;
    gapToTop++;

    std::vector<Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( feature_sample_image.cols, 0 );
    obj_corners[2] = cvPoint( feature_sample_image.cols, feature_sample_image.rows ); obj_corners[3] = cvPoint( 0, feature_sample_image.rows );
    std::vector<Point2f> scene_corners(4);

    perspectiveTransform( obj_corners, scene_corners, H);
    //cerr << "drawing box on image" << endl;
 
    int min_col = 125;
    int num = (i+1)*255.0*(1001001)/feature_samples.size();

    int R = 0, G = 0, B = 0;
    if(i%6 == 0 || i%6 == 3 || i%6 ==4)  { R = (255-min_col)*(1-(i*feature_samples.size()/6.0+1)+min_col);}
    if(i%6 == 1 || i%6 == 3 || i%6 ==5)  { G = (255-min_col)*(1-(i*feature_samples.size()/6.0+1)+min_col);}
    if(i%6 == 2 || i%6 == 4 || i%6 ==5)  { B = (255-min_col)*(1-(i*feature_samples.size()/6.0+1)+min_col);}


    line( image_clone, scene_corners[0], scene_corners[1], Scalar(  R, G, B ), 4 );
    line( image_clone, scene_corners[1], scene_corners[2], Scalar(  R, G, B ), 4 );
    line( image_clone, scene_corners[2], scene_corners[3], Scalar(  R, G, B ), 4 );
    line( image_clone, scene_corners[3], scene_corners[0], Scalar(  R, G, B ), 4 );

    //-- Show detected matches
    imshow( "matches", image_clone );
//    waitKey(0);
  }



  vector<Pattern_Result> results;
  return results;
}
