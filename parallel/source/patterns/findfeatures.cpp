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


class Count_Order {
  private:
    int count;
  public:
    bool operator()(double i, double j) { if(i<=j) {count++; return true;} else {return false;}}
    int reorderings() {return count;}
    Count_Order() {
      count = 0;
    }
};

int compare_rows(float x[], float y[], int x_len, int y_len) {
  int i=0;
  int j=0;
  
  vector<double> unordered;
  double minimum[2] = {0,DBL_MAX}, maximum[2] = {0,0};
  for(int i=0; i<x_len; i++) {
    cout << i << " " << x_len << endl;
    bool add = false;
    for(int j=0; j<y_len; j++) {
      if(fabs(x[i] - y[j]) < 0.1) {
        add = true;
        break;
      }
    }
    if(add) {
      unordered.push_back(x[i]);
      if(minimum[1] > x[i]) { 
        minimum[0] = i;
        minimum[1] = x[i];
      }
      if(maximum[1] < x[i]) { 
        maximum[0] = i;
        maximum[1] = x[i];
      }
    }
  }
  int count = maximum[0]-minimum[0];
  Count_Order co;
  sort(unordered.begin(), unordered.end(), co);
  return count-co.reorderings();
}

vector<DMatch> slow_image(vector<KeyPoint> object_keypoints,vector<KeyPoint> scene_keypoints) {
  cout << "starting slow" << endl;
  vector<DMatch> match_list;
  cout << scene_keypoints.size() << " " << object_keypoints.size() << endl;
  float scene_distance[scene_keypoints.size()][scene_keypoints.size()];
  float object_distance[object_keypoints.size()][object_keypoints.size()];
  cout << "made arrays" << endl;
  
  for(int i=0; i<scene_keypoints.size(); i++) {
    for(int j=0; j<scene_keypoints.size(); j++) {
      scene_distance[i][j] = dist(scene_keypoints[i].pt, scene_keypoints[j].pt);
    }
  }

  cout << "made scene" << endl;
  for(int i=0; i<object_keypoints.size(); i++) {
    for(int j=0; j<object_keypoints.size(); j++) {
      object_distance[i][j] = dist(object_keypoints[i].pt, object_keypoints[j].pt);
    }
  }

  cout << "made object" << endl;
  int match_quality[object_keypoints.size()][scene_keypoints.size()];
  for(int i=0; i<object_keypoints.size(); i++) {
    for(int j=0; j<scene_keypoints.size(); j++) {
      match_quality[i][j] = 0;
    }
  }

  cout << "comparing rows" << endl;
  for(int i=0; i<object_keypoints.size(); i++) {
    for(int j=0; j<scene_keypoints.size(); j++) {
      cout << "rows:" << i << " " << j << endl;
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
  cout << "returning" << endl;
  return match_list;
}

vector<DMatch> get_image_location(vector<KeyPoint> object_keypoints, vector<KeyPoint> scene_keypoints) {
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

vector<DMatch> compare_scales(vector<KeyPoint> object, vector<KeyPoint> scene, vector<DMatch> match_list, double max_scale) {
  vector<DMatch>::iterator x,y;
  map< double, vector<DMatch> > good_match_list;
  for(x=match_list.begin(); x!=match_list.end(); ++x) {
    y=x+1;
    for(; y!=match_list.end(); ++y) {
      Point2f a, b, c,d;    
      a = object[x->queryIdx].pt;
      b = object[y->queryIdx].pt;
      c = scene[x->trainIdx].pt;
      d = scene[y->trainIdx].pt;

      double object_distance = sqrt(pow(a.x-b.x,2)+(a.y-b.y,2));
      double scene_distance = sqrt(pow(c.x-d.x,2)+(c.y-d.y,2));
      double scale = object_distance/scene_distance;
      if(scale <= floor(max_scale)+1) {
        good_match_list[floor(scale*1000)/1000.0].push_back(*y);
      }
    }
  }
  map<int,vector<DMatch> > size_list;
  map<double,vector<DMatch> >:: iterator it=good_match_list.begin();
  for(; it!=good_match_list.end(); ++it) { 
    size_list[it->second.size()] = it->second;
  }
  return size_list.rbegin()->second;
}


vector<Pattern_Result> Find_Features::start_search(Mat image) {
  stringstream output_stream;
  print_output("starting", omp_get_thread_num(), omp_get_num_threads(), info.get_name());

  double scene_thickness = estimate_black_line_thickness(image,50,50);

  vector<Pattern_Result> results;
  vector <string> objects;

  // add all the potential objects to be searched over to the objects list
  //objects.push_back("feature_samples/wally_from_beach_no_background.png");
  objects.push_back("feature_samples/wally_from_shopping_mall_no_background.png");
  //objects.push_back("feature_samples/wally_from_hometown_no_background.png");
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
  for(int subimage=0; subimage<omp_get_max_threads(); subimage++) {
    subimage_size.width = image.cols/split[0];
    subimage_size.height = image.rows/split[1];
    subimage_center.x = image.cols*((1+split[0]*((float)(subimage))/split[0]))/(2*split[0]);
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
      vector<DMatch> match_list, good_match_list;
      vector<KeyPoint> object_keypoints;
      Mat object_image = imread(objects[i], CV_LOAD_IMAGE_COLOR);
      double object_thickness = estimate_black_line_thickness(object_image,50,50);
      double scale = object_thickness/scene_thickness;
      image_mask = get_colour_in_image(object_image, "#FF00FF", "#FF00FF", 1, 0, 1, 1, 0, 1);
      image_mask = image_mask != 255;
      print_output("getting object descriptors", omp_get_thread_num(), omp_get_num_threads(), info.get_name());

      sift(object_image, image_mask, object_keypoints, object_descriptors);
//      matcher.match(object_descriptors, scene_descriptors, match_list);
      good_match_list = slow_image(object_keypoints, scene_keypoints);
//      good_match_list = compare_scales(object_keypoints,scene_keypoints, match_list, scale);
      cout << good_match_list.size() << endl;
      Mat test;
      namedWindow("i", CV_WINDOW_AUTOSIZE);
      drawMatches(object_image, object_keypoints, scene_image, scene_keypoints, good_match_list, test);
      imshow("i", test);
      waitKey(0);
/*
      double mind=DBL_MAX,maxd=0;
      double mean_distance = 0;
      for(int i=0; i<match_list.size(); i++) {
        mean_distance += match_list[i].distance;
        if(match_list[i].distance > maxd) {maxd=match_list[i].distance;}
        if(match_list[i].distance < mind) {mind=match_list[i].distance;}
      }
      mean_distance/=match_list.size();
      double standard_deviation = 0;
      for(int i=0; i<match_list.size(); i++) {
        standard_deviation += pow(match_list[i].distance-mean_distance,2);
      }
      standard_deviation = sqrt(standard_deviation/match_list.size());
      cout << mean_distance << " " << standard_deviation << endl;
      cout << maxd << " " << mind << endl;
      for(int i=0;i<match_list.size(); i++) {
        if(fabs(match_list[i].distance-mean_distance) < standard_deviation) {
          good_match_list.push_back(match_list[i]);
        }
      }      
*/
/*
      namedWindow("i", CV_WINDOW_AUTOSIZE);
      imshow("i", object_image);
      waitKey(0);
      imshow("i", image_mask);
      waitKey(0);

      Mat test;
      cout << match_list.size() << endl;
      cout << scene_keypoints.size() << " " << object_keypoints.size() << endl;
      drawMatches(object_image, object_keypoints, scene_image, scene_keypoints, match_list, test);
      imshow("i", test);
      waitKey(0);
*/
  
/*
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

      vector<DMatch> match_list = slow_image(object_keypoints, scene_keypoints);
      //vector<DMatch> match_list = get_image_location(object_keypoints, scene_keypoints);

      if(match_list.size() == 0) {
        continue;
      }
*/
/*
      map<double,int> point_slope;
      vector<Point2f> obj, scene;
      Mat avg_H(3,3,CV_32F, Scalar(0));
      for(int j=0; j<match_list.size(); j++) {
        Point2f a = object_keypoints[match_list[j].trainIdx].pt;
        Point2f b = scene_keypoints[match_list[j].queryIdx].pt;
        double slope = (a.y-b.y)/(a.x-b.x);
        slope = floor(slope*10)/10.0;
        point_slope[slope] += 1;
      }

      double most_common_slope = 0;
      int slope_count = 0;
      cout << match_list.size() << " " << point_slope.size() << endl;
      for(map<double,int>::iterator it=point_slope.begin(); it!=point_slope.end(); ++it) {
        if(it->second > slope_count) {
          most_common_slope = it->first;
          slope_count = it->second;
        }
      }
      cout << most_common_slope << " " << slope_count << endl;

      for(int j=0; j<match_list.size(); j++) {  
        point2f a = object_keypoints[match_list[j].trainidx].pt;
        point2f b = scene_keypoints[match_list[j].queryidx].pt;
        double slope = (a.y-b.y)/(a.x-b.x);
        slope = floor(slope*10)/10.0;
        if(fabs(slope-most_common_slope) < 0.1) {
          scene.push_back(object_keypoints[match_list[j].trainidx].pt);
          obj.push_back(scene_keypoints[match_list[j].queryidx].pt);
        }
      }
      mat h = findhomography(obj, scene,cv_ransac);
      cout << H << endl;
*/
      vector<Point2f> obj, scene;
      for(int j=0; j<good_match_list.size(); j++) {  
        scene.push_back(object_keypoints[good_match_list[j].trainIdx].pt);
        obj.push_back(scene_keypoints[good_match_list[j].queryIdx].pt);
      }
      Mat H = findHomography(obj, scene, CV_RANSAC);
      cout << H << endl;

      vector<Point2f> obj_corners(4), scene_corners(4);
      obj_corners[0] = cvPoint(0,0);
      obj_corners[1] = cvPoint( object_image.cols, 0 );
      obj_corners[2] = cvPoint( object_image.cols, object_image.rows );
      obj_corners[3] = cvPoint( 0, object_image.rows );

      print_output("creating perspective transform", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
      perspectiveTransform( obj_corners, scene_corners, H);
//      if(homography_quality(H) < 0.01) {
        Pattern_Result tmp;
        tmp.info = info;
        tmp.wally_location[0] = (scene_corners[0].x+scene_corners[1].x)/2+subimage_center.x-image.rows/4;
        tmp.wally_location[1] = (scene_corners[0].y+scene_corners[2].y)/2+subimage_center.y-image.cols/4;
        tmp.scale[0] = (scene_corners[2].x-scene_corners[0].x)/2;
        tmp.scale[1] = (scene_corners[2].y-scene_corners[0].y)/2;
        cout << H.at<double>(2,0) << " " << H.at<double>(2,1) << endl;
        tmp.certainty = 1-fabs(H.at<double>(2,0)*H.at<double>(2,1))-fabs(H.at<double>(2,0)) -fabs(H.at<double>(2,1));
//        if(tmp.scale[0] > 0 && tmp.scale[1] > 0)
//        {
//          #pragma omp critical
//          {
            results.push_back(tmp);
//          }
//        }
  //    }
      object_image.release();
      print_output("done", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
    }
    scene_image.release();
  }
  print_output("done", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  return results;
}
