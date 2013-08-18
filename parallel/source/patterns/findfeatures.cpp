#include <whereswally/patterns.h>
#include <omp.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cfloat>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;
using namespace wwp;

// initialise Find_Features class
Find_Features::Find_Features() {
  info.set_name("Find Features");
  info.set_description("Locate Wally by SURF feature matching");
  info.set_confidence(0.9);
}

//--                                   --//
//--                                   --//
//--  Helper Functions and Structures  --//
//--                                   --//
//--                                   --//

// identify best quality homography
double Find_Features::homography_quality(homography H) {
  if(H.a <= 0 || H.c <= 0) {
    return 10;
  }
  double scale = H.a/H.c;
  double min_scale = min(scale,1/scale);
  return fabs(1-min_scale);
}


// calculates the euclidean distance between two points
double Find_Features::dist(Point2f pt1, Point2f pt2) {
  return sqrt((pow(pt1.x-pt2.x,2)) + (pow(pt1.y-pt2.y,2)));
}

double Find_Features::dist(Point2f pt) {
  return sqrt(pow(pt.x,2) + (pow(pt.y,2)));
}

// counts the elements of a vector that are ordered
int Find_Features::count_order(vector<double> list) {
  int count=0;
  vector<double>::iterator it=list.begin();
  int old_val = *it;
  ++it;
  while(it!=list.end()) {
    if(old_val < *it) {
      ++count; 
    }
    old_val = *it;
    ++it;
  }
  return count+1; // count+1 because last element is always ordered
}

// Calculates how many ordered elements parts row y has in common with row x
// This is done by creating a list of each element that x has in common with y
// This order of this list is then calculated using 'count_order'
/* 
 * EXAMPLES
 *
 *  x = (0,1,2),  y = (0,1,2),        commonality = 3
 *  x = (0,2,1),  y = (0,2,1,3,4),    commonality = 3
 *  x = (0,1,2),  y = (0,1,2,0,1,2),  commonality = 3
 *  x = (0,1,2),  y = (2,1,0),        commonality = 1
 *  x = (0,2),    y = (0,1,2),        commonality = 2
 *  x = (0,2,1),  y = (0,1,2),        commonality = 2
 *
 */
int Find_Features::compare_rows(float x[], float y[], int x_len, int y_len) {
  vector<double> unordered;
  for(int i=0; i<x_len; i++) {
    bool add = false;
    for(int j=0; j<y_len; j++) {
      if(fabs(x[i] - y[j]) < 0.1) {
        unordered.push_back(x[i]);
        break;
      }
    }
  }
  return count_order(unordered);
}

// creates an artifical homography that can match 
homography Find_Features::calculate_scale_and_translation(vector< vector<DMatch > > matches, vector<KeyPoint> object_keypoints, vector<KeyPoint> scene_keypoints, int i, int j, int k, int l) {
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

//--                               --//
//--                               --//
//--  Methods for finding matches  --//
//--                               --//
//--                               --//

vector<DMatch> Find_Features::find_matches_with_equivalent_distances(vector<KeyPoint> object_keypoints,vector<KeyPoint> scene_keypoints, Mat object_descriptors, Mat scene_descriptors) {
  BFMatcher matcher;
  vector<DMatch> match_list;

  matcher.match(object_descriptors, scene_descriptors, match_list);
  float scene_distance[scene_keypoints.size()][scene_keypoints.size()];
  float object_distance[object_keypoints.size()][object_keypoints.size()];
  
  for(int i=0; i<scene_keypoints.size(); i++) {
    for(int j=0; j<scene_keypoints.size(); j++) {
      scene_distance[i][j] = dist(scene_keypoints[i].pt, scene_keypoints[j].pt);
    }
  }

  for(int i=0; i<object_keypoints.size(); i++) {
    for(int j=0; j<object_keypoints.size(); j++) {
      object_distance[i][j] = dist(object_keypoints[i].pt, object_keypoints[j].pt);
    }
  }

  int match_quality[object_keypoints.size()][scene_keypoints.size()];
  for(int i=0; i<object_keypoints.size(); i++) {
    for(int j=0; j<scene_keypoints.size(); j++) {
      match_quality[i][j] = 0;
    }
  }

  for(int i=0; i<object_keypoints.size(); i++) {
    for(int j=0; j<scene_keypoints.size(); j++) {
      match_quality[i][j] = compare_rows(object_distance[i], scene_distance[j], object_keypoints.size(), scene_keypoints.size());
    }
  }

  for(int i=0; i<object_keypoints.size(); i++) {
    int max_match[2]={0,0};
    for(int j=0; j<scene_keypoints.size(); j++ ){
      if(match_quality[i][j] > max_match[0]) { max_match[0] = match_quality[i][j]; max_match[1] = j;}
    }
    DMatch tmp;
    tmp.queryIdx = i;
    tmp.trainIdx = max_match[1];
    match_list.push_back(tmp);
  }
  return match_list;
}

vector<DMatch> Find_Features::find_matches_with_equivalent_ratios(vector<KeyPoint> object_keypoints,vector<KeyPoint> scene_keypoints, Mat object_descriptors, Mat scene_descriptors) {
  BFMatcher matcher;
  vector<DMatch> match_list;

  matcher.match(object_descriptors, scene_descriptors, match_list);
  float scene_distance[scene_keypoints.size()][scene_keypoints.size()];
  float object_distance[object_keypoints.size()][object_keypoints.size()];
  
  for(int i=0; i<scene_keypoints.size(); i++) {
    for(int j=0; j<scene_keypoints.size(); j++) {
      scene_distance[i][j] = dist(scene_keypoints[j].pt)/dist(scene_keypoints[i].pt);
    }
  }

  for(int i=0; i<object_keypoints.size(); i++) {
    for(int j=0; j<object_keypoints.size(); j++) {
      object_distance[i][j] = dist(object_keypoints[j].pt)/dist(object_keypoints[i].pt);
    }
  }

  int match_quality[object_keypoints.size()][scene_keypoints.size()];
  for(int i=0; i<object_keypoints.size(); i++) {
    for(int j=0; j<scene_keypoints.size(); j++) {
      match_quality[i][j] = 0;
    }
  }

  for(int i=0; i<object_keypoints.size(); i++) {
    for(int j=0; j<scene_keypoints.size(); j++) {
      match_quality[i][j] = compare_rows(object_distance[i], scene_distance[j], object_keypoints.size(), scene_keypoints.size());
    }
  }

  for(int i=0; i<object_keypoints.size(); i++) {
    int max_match[2]={0,0};
    for(int j=0; j<scene_keypoints.size(); j++ ){
      if(match_quality[i][j] > max_match[0]) { max_match[0] = match_quality[i][j]; max_match[1] = j;}
    }
    DMatch tmp;
    tmp.queryIdx = i;
    tmp.trainIdx = max_match[1];
    match_list.push_back(tmp);
  }
  return match_list;
}

// finds first-degree matches between two descriptors 
vector<DMatch> Find_Features::find_matches_with_matcher(Mat object_descriptors, Mat scene_descriptors) {
  BFMatcher matcher;
  vector<DMatch> match_list;

  matcher.match(object_descriptors, scene_descriptors, match_list);
  return match_list;
}

// finds matches such that each keypoint in a pair is the best match of the other keypoint 
vector<DMatch> Find_Features::find_symmetric_matches_with_matcher(Mat object_descriptors, Mat scene_descriptors) {
  BFMatcher matcher(NORM_L2, true);
  vector<DMatch> match_list;

  matcher.match(object_descriptors, scene_descriptors, match_list);
  return match_list;
}

// finds n-degree matches between two descriptors
vector< vector<DMatch> > Find_Features::find_n_degree_matches_with_matcher(Mat object_descriptors, Mat scene_descriptors, int n) {
  BFMatcher matcher;
  vector< vector<DMatch> > match_list;

  matcher.knnMatch(object_descriptors, scene_descriptors, match_list, n);
  return match_list;
}

//--                                                                        --//
//--                                                                        --//
//--  Methods to create transformation matrix (homography) between matches  --//
//--                                                                        --//
//--                                                                        --//


Mat Find_Features::find_homography_within_minimum_distance(vector<KeyPoint> object_keypoints, vector<KeyPoint> scene_keypoints, vector<DMatch> match_list, float tolerance) {
  double min_distance = DBL_MAX;
  vector<Point2f> object_position, scene_position;
  for(int i=0; i<match_list.size(); i++) {
    double distance = match_list[i].distance;
    if(distance < min_distance) {
      min_distance = distance;
    }
  }

  for(int i=0; i<match_list.size(); i++) {
    double distance = match_list[i].distance;
    if(tolerance == 0 || distance < tolerance*min_distance) {
      object_position.push_back(object_keypoints[match_list[i].queryIdx].pt);
      scene_position.push_back(scene_keypoints[match_list[i].trainIdx].pt);
    }
  }
  if(object_position.size() < 4) {
    return Mat(3,3,CV_32F,Scalar(1));
  }
  return findHomography(object_position, scene_position, CV_RANSAC);
}

Mat Find_Features::find_homography_with_translational_invariance(vector<KeyPoint> object_keypoints, vector<KeyPoint> scene_keypoints, vector< vector <DMatch> > match_list) {
  vector<homography> homography_list;
  map<homography, vector<DMatch> > scale_and_translation_matches;
  
  print_output("creating transform fitted matches", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  for(int j=0; j<match_list.size(); j++) {
    for(int k=0; k<match_list[j].size(); k++) {
      for(int l=j+1; l<match_list.size(); l++) {
        for(int m=0; m<match_list[l].size(); m++) {
          homography H = calculate_scale_and_translation(match_list, object_keypoints, scene_keypoints, j,k,l,m);
          homography_list.push_back(H);
          scale_and_translation_matches[H].push_back(match_list[j][k]);
          scale_and_translation_matches[H].push_back(match_list[l][m]);
        }
      }
    }
  }

  homography min_H;
  min_H.a = 0;
  min_H.b = 0;
  min_H.c = 0;
  min_H.d = 0;

  for(int j=0; j<homography_list.size(); j++) {
    if(homography_quality(homography_list[j]) < homography_quality(min_H)) {
      min_H = homography_list[j];
    }
  }

  float content[9] = {min_H.a,0,min_H.b,0,min_H.c,min_H.d,0,0,1};
  Mat H(3,3,CV_32F, content);
  return H;
}

//--                                                           --//
//--                                                           --//
//--  Functions that matches the scene image to object images  --//
//--                                                           --//
//--                                                           --//

vector<Pattern_Result> Find_Features::start_search(Mat image) {
  stringstream output_stream;
  print_output("starting", omp_get_thread_num(), omp_get_num_threads(), info.get_name());

  double scene_thickness = estimate_black_line_thickness(image,50,50);

  vector<Pattern_Result> results;
  vector <string> objects;

  // add all the potential objects to be searched over to the objects list
  objects.push_back("feature_samples/wally_from_beach_no_background.png");
  objects.push_back("feature_samples/wally_from_beach_with_background.png");
  objects.push_back("feature_samples/wally_from_shopping_mall_no_background.png");
  objects.push_back("feature_samples/wally_from_shopping_mall_with_background.png");
  objects.push_back("feature_samples/wally_from_hometown_no_background.png");
  //objects.push_back("feature_samples/wally_large_generic.png");

  int largest_object_width = 102; //hand checked
  int largest_object_height = 162; // hand checked

  // calculate best way to split image up amongst processes
  int num_threads = omp_get_max_threads();
  int size[2] = {image.cols, image.rows};
  int split[2] = {1,1};
  int tmp_num_threads = num_threads;
  while(tmp_num_threads > 1) {
    int div = 2;
    while(tmp_num_threads%2!=0) {
      div++;
    }
    tmp_num_threads /= div;
    int largest = 0;
    if((float)(size[1])/split[1] > (float)(size[0])/split[0]) {
      largest = 1;
    }
    split[largest] *= div;
  }

  #pragma omp parallel for default(none) shared(image, objects, results) firstprivate(scene_thickness, num_threads, split)
  for(int subimage=0; subimage<num_threads; ++subimage) {
    Mat scene_descriptors;
    vector<KeyPoint> scene_keypoints;
    Size subimage_size;
    Point2f subimage_center;
    SIFT sift;

    int x[2] = { (subimage/split[0])*(image.rows-1)/split[1], (subimage/split[0]+1)*(image.rows-1)/split[1] };
    int y[2] = { (subimage%split[0])*(image.cols-1)/split[0], (subimage%split[0]+1)*(image.cols-1)/split[0] };

    //Mat scene_image(subimage_size, image.type());
    Mat scene_image = image(
      Range(x[0],x[1]),
      Range(y[0],y[1])
    );

    sift(scene_image, Mat(), scene_keypoints, scene_descriptors);

    print_output("preparing to analyse objects", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
    for(int i=0; i<objects.size(); i++) {
      Mat object_descriptors, image_mask, object_image;
      vector<KeyPoint> object_keypoints;
      vector<DMatch> match_list;
      double object_thickness, certainty;
      double scale;

      object_image = imread(objects[i], CV_LOAD_IMAGE_COLOR);
      object_thickness = estimate_black_line_thickness(object_image,50,50);
      scale = floor(floor(object_thickness+1)/floor(scene_thickness+1)+0.5);

      image_mask = get_colour_in_image(object_image, "#FF00FF", "#FF00FF", 1, 0, 1, 1, 0, 1);
      image_mask = image_mask != 255;
      print_output("getting object descriptors", omp_get_thread_num(), omp_get_num_threads(), info.get_name());

      sift(object_image, image_mask, object_keypoints, object_descriptors);

      match_list = find_matches_with_matcher(object_descriptors, scene_descriptors);
      //    match_list = find_matches_with_equivalent_ratios(object_keypoints,scene_keypoints, object_descriptors, scene_descriptors);
      Mat H = find_homography_within_minimum_distance(object_keypoints, scene_keypoints, match_list,0);
      // certainty decided by how close the elements H_{2,0} and H_{2,1} are to their expected value, zero
      certainty = pow(1-fabs(H.at<double>(2,0)*H.at<double>(2,1))-fabs(H.at<double>(2,0)) -fabs(H.at<double>(2,1)),16);

      vector<Point2f> obj_corners(4), scene_corners(4);
      obj_corners[0] = cvPoint(0,0);
      obj_corners[1] = cvPoint( object_image.cols, 0 );
      obj_corners[2] = cvPoint( object_image.cols, object_image.rows );
      obj_corners[3] = cvPoint( 0, object_image.rows );

      print_output("creating perspective transform", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
      perspectiveTransform( obj_corners, scene_corners, H);

      Pattern_Result tmp;
      tmp.info = info;
      tmp.wally_location[0] = (scene_corners[0].x+scene_corners[1].x)/2+y[0];
      tmp.wally_location[1] = (scene_corners[0].y+scene_corners[2].y)/2+x[0];
      tmp.scale[0] = (scene_corners[2].x-scene_corners[0].x)/2;
      tmp.scale[1] = (scene_corners[2].y-scene_corners[0].y)/2;
      tmp.certainty = certainty;
      if(tmp.certainty >= 0 && tmp.scale[0] > 0 && tmp.scale[1] > 0 && tmp.scale[0] <= scale*object_image.rows && tmp.scale[1] <= scale*object_image.cols)
      {
        #pragma omp critical
        {
          results.push_back(tmp);
        }
      }
      object_image.release();
      print_output("done", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
    }
    scene_image.release();
  }
  print_output("done", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  return results;
}
