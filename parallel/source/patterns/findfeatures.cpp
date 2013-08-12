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

typedef struct {
  float intersections;
  int i;
  int j;
} triple;

Find_Features::Find_Features() {
  info.set_name("Find Features");
  info.set_description("Locate Wally by SURF feature matching");
  info.set_confidence(0.9);
  minHessian = 100;
  knn_depth = 1;
}

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

double dist(Point2f pt1, Point2f pt2) {
  float accuracy = 100.0;
  return floor((int)(sqrt((pow(pt1.x-pt2.x,2)) + (pow(pt1.y-pt2.y,2)))*accuracy))/accuracy;
}

vector<DMatch> slow_image(vector<KeyPoint> scene_keypoints, vector<KeyPoint> object_keypoints) {
  vector<DMatch> match_list;
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
      multiset<float> obj;
      for(int k=0; k<object_keypoints.size(); k++) {
        obj.insert(object_distance[i][k]);
      }
      for(int k=0; k<scene_keypoints.size(); k++) {
        float d = scene_distance[j][k];
        multiset<float>::iterator it = obj.find(d);
        if(it != obj.end()) {
          obj.erase(it);
          match_quality[i][j]++;
          
        }
      }
      
    }
  }

/*
  for(int i=0; i<object_keypoints.size(); i++) {
    for(int j=0; j<scene_keypoints.size(); j++) {
      for(int k=j+1; k<scene_keypoints.size(); k++) {
        for(int l=i+1; l<object_keypoints.size(); l++) {
          if(object_distance[i][l] == scene_distance[j][k]) {
            match_quality[i][j]++;
            match_quality[l][k]++;
          }
        }
      }
    }
  }
*/
  bool first = true;
  int max[3];
  do {
    max[0] = -1; max[1] = -1; max[2] = -1;
    for(int i=0; i<object_keypoints.size(); i++) {
      for(int j=0; j<scene_keypoints.size(); j++) {
        if(match_quality[i][j] > max[0]) {
          bool cont = false;
          for(int k=0; k<match_list.size(); k++) {
            if(i == match_list[k].trainIdx) {cont=true; break;}
            if(j == match_list[k].queryIdx) {cont=true; break;}
          }
          if(cont) {continue;}
          max[0] = match_quality[i][j];
          max[1] = i;
          max[2] = j;
        }
      }
    }
    if(max[0]*100.0/object_keypoints.size() <= 25) {break;}
    DMatch tmp;
    tmp.queryIdx = max[2];
    tmp.trainIdx = max[1];
    match_list.push_back(tmp);
  } while(true);
  
  return match_list;
}

vector<DMatch> get_image_location(vector<KeyPoint> scene_keypoints, vector<KeyPoint> object_keypoints) {
  vector< multimap<float, int> > best_match(object_keypoints.size());
  vector< multiset<float> > scene_distance(scene_keypoints.size()), object_distance(object_keypoints.size());

  // calculate the distances bet
  for(int i=0; i<scene_keypoints.size(); ++i) {
    for(int j=0; j<scene_keypoints.size(); ++j) {
      if(i==j) { continue; }
      scene_distance[i].insert(dist(scene_keypoints[i].pt,scene_keypoints[j].pt));
    }
  } 

  for(int i=0; i<object_keypoints.size(); ++i) {
    for(int j=0; j<object_keypoints.size(); ++j) {
      if(i==j) { continue; }
      object_distance[i].insert(dist(object_keypoints[i].pt,object_keypoints[j].pt));
    }
  } 

  vector<float>::iterator it;
  vector<float> keypoint_intersection;
  for(int i=0; i<object_distance.size(); i++) {
    for(int j=0; j<scene_distance.size(); j++) {
      keypoint_intersection.clear();
      keypoint_intersection.resize(object_distance[i].size());
      it = set_intersection(object_distance[i].begin(), object_distance[i].end(), scene_distance[j].begin(), scene_distance[j].end(), keypoint_intersection.begin()); 
      keypoint_intersection.resize(it-keypoint_intersection.begin());
      if(keypoint_intersection.size() >= 0*object_distance[i].size() && keypoint_intersection.size() > 0) {
        best_match[i].insert(pair<float,int>(keypoint_intersection.size()*100.0/object_distance[i].size(), j));
      }
    }
  }

  vector<DMatch> match_list;
  multimap<float,int>::reverse_iterator rit;
  triple max = {0,-1,-1}; 
  do {
    max.intersections = 0;
    max.i = -1;
    max.j = -1;
    for(int i=0; i<best_match.size(); ++i) {
      rit = best_match[i].rbegin();
      for(rit=best_match[i].rbegin(); rit!=best_match[i].rend(); ++rit) {
        if(rit->first > max.intersections) {
          bool cont = false;
          for(int j=0; j<match_list.size(); j++) {
            if(i == match_list[j].trainIdx) {cont = true;}
          }
          if(cont) {continue;}
          max.intersections = rit->first;
          max.i = i;
          max.j = rit->second;
          break;
        }
      }
    }
    if( max.intersections <= 0) {break;}
    DMatch tmp;
    tmp.queryIdx = max.j;
    tmp.trainIdx = max.i;
    match_list.push_back(tmp);
  } while(true);
  cout << endl;
  cout << "match_list size = " << match_list.size() << endl;
  cout << endl;

  return match_list;
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
vector<Pattern_Result> Find_Features::start_search(Mat image) {
  stringstream output_stream;
  print_output("starting", omp_get_thread_num(), omp_get_num_threads(), info.get_name());

  vector<Pattern_Result> results;
  vector <string> objects;

  // add all the potential objects to be searched over to the objects list
  objects.push_back("feature_samples/wally_from_beach_no_background.png");
  objects.push_back("feature_samples/wally_from_shopping_mall_no_background.png");
  objects.push_back("feature_samples/wally_from_hometown_no_background.png");
  //objects.push_back("feature_samples/wally_large_generic.png");

  int largest_object_width = 102; //hand checked
  int largest_object_height = 162; // hand checked

  // calculate best way to split image up amongst processes
  int num_threads = omp_get_num_threads();
  int size[2] = {image.cols, image.rows};
  int split[2] = {1,1};
  while(num_threads > 1) {
    int div = 2;
    while(num_threads%2!=0) {
      div++;
    }
    num_threads /= div;
    int largest = 0;
    if((float)(size[1])/split[1] > (float)(size[0])/split[0]) {
      largest = 1;
    }
    split[largest] *= div;
  }

  BFMatcher matcher;
  SIFT sift;
  Size subimage_size;
  Point2f subimage_center;
//  #pragma omp parallel for 
  for(int subimage=0; subimage<omp_get_num_threads(); subimage++) {
    subimage_size.width = image.cols/split[0];
    subimage_size.height = image.rows/split[1];
    subimage_center.x = image.cols*(1+split[0]*(subimage/split[0]))/(2*split[0]);
    subimage_center.y = image.rows*(1+split[1]*(subimage%split[1]))/(2*split[1]);

    Mat scene_image(subimage_size, image.depth());
    getRectSubPix(image, subimage_size, subimage_center, scene_image);
  
    stringstream out;
    out << "generating keypoints in scene (" << subimage_center.x << "," << subimage_center.y << ")";
    print_output(out.str(), omp_get_thread_num(), omp_get_num_threads(), info.get_name());

    Mat scene_descriptors, object_descriptors;
    Mat image_mask(scene_image.rows, scene_image.rows, CV_8U, Scalar(255));
    vector<KeyPoint> scene_keypoints;
    sift(scene_image, Mat(), scene_keypoints, scene_descriptors);

    for(int i=0; i<objects.size(); i++) {
      vector<homography> homography_list;
      vector< vector< DMatch > > matches, good_matches;

      Mat object_image = imread(objects[i], CV_LOAD_IMAGE_COLOR);
      vector<KeyPoint> object_keypoints;
      map< homography, vector< DMatch> > scale_and_translation_matches;
      image_mask = get_colour_in_image(object_image, "#FF00FF", "#FF00FF", 1, 0, 1, 1, 0, 1);
      image_mask = image_mask < 125;
      print_output("getting object descriptors", omp_get_thread_num(), omp_get_num_threads(), info.get_name());

      sift(object_image, image_mask, object_keypoints, object_descriptors);

/*
      Mat test;
      cout << match_list.size() << endl;
      drawMatches(scene_image, scene_keypoints, object_image, object_keypoints, match_list, test);
      namedWindow("i", CV_WINDOW_AUTOSIZE);
      imshow("i", test);
      waitKey(0);
*/
  
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

/*
      vector<DMatch> match_list = slow_image(scene_keypoints, object_keypoints);
      //vector<DMatch> match_list = get_image_location(scene_keypoints, object_keypoints);

      if(match_list.size() == 0) {
        continue;
      }
      vector<Point2f> obj, scene;
      for(int j=0; j<match_list.size(); j++) {
        obj.push_back(object_keypoints[match_list[j].trainIdx].pt);
        scene.push_back(scene_keypoints[match_list[j].queryIdx].pt);
      }
      cout << obj.size() << " " << scene.size() << endl;
      Mat H = findHomography(obj, scene);
      cout << H << endl;
*/
      vector<Point2f> obj_corners(4), scene_corners(4);
      obj_corners[0] = cvPoint(0,0);
      obj_corners[1] = cvPoint( object_image.cols, 0 );
      obj_corners[2] = cvPoint( object_image.cols, object_image.rows );
      obj_corners[3] = cvPoint( 0, object_image.rows );

      print_output("creating perspective transform", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
      perspectiveTransform( obj_corners, scene_corners, H);
      cout << "hiya" << endl;
//      if(if(homography_quality(H) < 0.01) {
        Pattern_Result tmp;
        tmp.info = info;
        tmp.wally_location[0] = (scene_corners[0].x+scene_corners[1].x)/2+subimage_center.x-image.rows/4;
        tmp.wally_location[1] = (scene_corners[0].y+scene_corners[2].y)/2+subimage_center.y-image.cols/4;
        tmp.scale[0] = (scene_corners[2].x-scene_corners[0].x)/2;
        tmp.scale[1] = (scene_corners[2].y-scene_corners[0].y)/2;
        tmp.certainty = fabs(1-determinant(H));
        if(tmp.scale[0] > 0 && tmp.scale[1] > 0)
        {
          cout << "part1" << endl;
          #pragma omp critical
          {
            results.push_back(tmp);
          }
          cout << "part2" << endl;
        }
  //    }
      print_output("done", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
    }
    cout << "chums" << endl;
  }

  print_output("done", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  return results;
}
