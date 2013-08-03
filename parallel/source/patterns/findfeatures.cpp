#include <whereswally/patterns.h>
#include <omp.h>
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

// find wally using sift techniques
vector<Pattern_Result> Find_Features::start_search(Mat scene_image) {
  stringstream output_stream;
  print_output("starting", omp_get_thread_num(), omp_get_num_threads(), info.get_name());

  vector<Pattern_Result> results;
  vector <string> objects;
  Mat scene_descriptors, object_descriptors;
  FlannBasedMatcher matcher;
  SIFT sift;

  // add all the potential objects to be searched over to the objects list
  objects.push_back("feature_samples/wally_from_beach_no_background.png");
  objects.push_back("feature_samples/wally_from_shopping_mall_no_background.png");
  //objects.push_back("feature_samples/wally_large_generic.png");

  print_output("generating keypoints in scene", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  Mat image_mask(scene_image.rows, scene_image.rows, CV_8U, Scalar(255));
  vector<KeyPoint> scene_keypoints;
  sift(scene_image, image_mask, scene_keypoints, scene_descriptors);

  print_output("entering parallel region", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  #pragma omp parallel for shared(scene_keypoints, scene_descriptors, sift, results, objects)
  for(int i=0; i<objects.size(); i++) {
    time_t t = time(NULL);
    vector<homography> homography_list;
    vector< vector< DMatch > > matches, good_matches;

    Mat object_image = imread(objects[i], CV_LOAD_IMAGE_COLOR);
    vector<KeyPoint> object_keypoints;
    map< homography, vector< DMatch> > scale_and_translation_matches;
    image_mask = get_colour_in_image(object_image, "#FF00FF", "#FF00FF", 1, 0, 1, 1, 0, 1);
    image_mask = image_mask < 125;
    print_output("getting object descriptors", omp_get_thread_num(), omp_get_num_threads(), info.get_name());

    sift(object_image, image_mask, object_keypoints, object_descriptors);
    matcher.knnMatch(object_descriptors, scene_descriptors, matches, knn_depth );

    print_output("creating transform fitted matches", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
    for(int j=0; j<matches.size(); j++) {
      for(int k=0; k<matches[j].size(); k++) {
        for(int l=j+1; l<matches.size(); l++) {
          for(int m=0; m<matches[l].size(); m++) {
            homography H = calculate_scale_and_translation(matches, object_keypoints, scene_keypoints, j,k,l,m);
            homography_list.push_back(H);
            scale_and_translation_matches[H].push_back(matches[j][k]);
            scale_and_translation_matches[H].push_back(matches[l][m]);
          }
        }
      }
    }

    homography min_H;
    min_H.a = 0;
    min_H.b = 0;
    min_H.c = 0;
    min_H.d = 0;

    print_output("analysing homography", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
    for(int j=0; j<homography_list.size(); j++) {
      if(homography_quality(homography_list[j]) < homography_quality(min_H)) {
        min_H = homography_list[j];
      }
    }

    float content[9] = {min_H.a,0,min_H.b,0,min_H.c,min_H.d,0,0,1};
    Mat H(3,3,CV_32F, content);

    vector<Point2f> obj_corners(4), scene_corners(4);
    obj_corners[0] = cvPoint(0,0);
    obj_corners[1] = cvPoint( object_image.cols, 0 );
    obj_corners[2] = cvPoint( object_image.cols, object_image.rows );
    obj_corners[3] = cvPoint( 0, object_image.rows );

    print_output("creating perspective transform", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
    perspectiveTransform( obj_corners, scene_corners, H);
    if(homography_quality(min_H) < 0.01) {
      Pattern_Result tmp;
      tmp.info = info;
      tmp.wally_location[0] = (scene_corners[0].x+scene_corners[1].x)/2;
      tmp.wally_location[1] = (scene_corners[0].y+scene_corners[2].y)/2;
      tmp.scale[0] = (scene_corners[2].x-scene_corners[0].x)/2;
      tmp.scale[1] = (scene_corners[2].y-scene_corners[0].y)/2;
      tmp.certainty = (1-homography_quality(min_H));
      if(tmp.scale[0] > 0 && tmp.scale[1] > 0)
      {
        #pragma omp critical
        results.push_back(tmp);
      }
    }
    stringstream tmp;
    print_output("done", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  }

  print_output("done", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  return results;
}
