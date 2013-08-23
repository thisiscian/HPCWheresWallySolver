#include <whereswally/patterns.h>
using namespace std;
using namespace cv;
using namespace wwp;

//**
//**  CLASS INITIALISATION
//** 

Find_Features::Find_Features() {
  info.set_name("Find Features");
  info.set_description("Locate Wally by SURF feature matching");
  info.set_confidence(0.9);
}

//**
//** HELPER FUNCTIONS
//**

//-- calculates the euclidean distance between two points
double Find_Features::dist(Point2f pt1, Point2f pt2) {
  return sqrt((pow(pt1.x-pt2.x,2)) + (pow(pt1.y-pt2.y,2)));
}

//-- calculates the euclidean distance between a point and the origin
double Find_Features::dist(Point2f pt) {
  return dist(pt, Point2f(0,0));
}

//-- counts the elements of a vector that are ordered
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
  return count+1; //-- count+1 because last element is always ordered
}

//-- Calculates how many ordered elements parts row y has in common with row x
//-- This is done by creating a list of each element that x has in common with y
//-- This order of this list is then calculated using 'count_order'
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
    for(int j=0; j<y_len; j++) {
      if(fabs(x[i] - y[j]) < 0.1) {
        unordered.push_back(x[i]);
        break;
      }
    }
  }
  return count_order(unordered);
}

//**
//** MATCHING METHODS
//**

vector<DMatch> Find_Features::find_matches_with_equivalent_distances(vector<KeyPoint> object_keypoints,vector<KeyPoint> scene_keypoints, Mat object_descriptors, Mat scene_descriptors) {
  BFMatcher matcher;
  vector<DMatch> match_list;

  matcher.match(object_descriptors, scene_descriptors, match_list);
  float scene_distance[scene_keypoints.size()][scene_keypoints.size()];
  float object_distance[object_keypoints.size()][object_keypoints.size()];
  
  for(size_t i=0; i<scene_keypoints.size(); i++) {
    for(size_t j=0; j<scene_keypoints.size(); j++) {
      scene_distance[i][j] = dist(scene_keypoints[i].pt, scene_keypoints[j].pt);
    }
  }

  for(size_t i=0; i<object_keypoints.size(); i++) {
    for(size_t j=0; j<object_keypoints.size(); j++) {
      object_distance[i][j] = dist(object_keypoints[i].pt, object_keypoints[j].pt);
    }
  }

  int match_quality[object_keypoints.size()][scene_keypoints.size()];
  for(size_t i=0; i<object_keypoints.size(); i++) {
    for(size_t j=0; j<scene_keypoints.size(); j++) {
      match_quality[i][j] = 0;
    }
  }

  for(size_t i=0; i<object_keypoints.size(); i++) {
    for(size_t j=0; j<scene_keypoints.size(); j++) {
      match_quality[i][j] = compare_rows(object_distance[i], scene_distance[j], object_keypoints.size(), scene_keypoints.size());
    }
  }

  for(size_t i=0; i<object_keypoints.size(); i++) {
    int max_match[2]={0,0};
    for(size_t j=0; j<scene_keypoints.size(); j++ ){
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
  
  for(size_t i=0; i<scene_keypoints.size(); i++) {
    for(size_t j=0; j<scene_keypoints.size(); j++) {
      scene_distance[i][j] = dist(scene_keypoints[j].pt)/dist(scene_keypoints[i].pt);
    }
  }

  for(size_t i=0; i<object_keypoints.size(); i++) {
    for(size_t j=0; j<object_keypoints.size(); j++) {
      object_distance[i][j] = dist(object_keypoints[j].pt)/dist(object_keypoints[i].pt);
    }
  }

  int match_quality[object_keypoints.size()][scene_keypoints.size()];
  for(size_t i=0; i<object_keypoints.size(); i++) {
    for(size_t j=0; j<scene_keypoints.size(); j++) {
      match_quality[i][j] = 0;
    }
  }

  for(size_t i=0; i<object_keypoints.size(); i++) {
    for(size_t j=0; j<scene_keypoints.size(); j++) {
      match_quality[i][j] = compare_rows(object_distance[i], scene_distance[j], object_keypoints.size(), scene_keypoints.size());
    }
  }

  for(size_t i=0; i<object_keypoints.size(); i++) {
    int max_match[2]={0,0};
    for(size_t j=0; j<scene_keypoints.size(); j++ ){
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

//**
//** HOMOGRAPHY GENERATORS
//**

Mat Find_Features::find_homography_within_minimum_distance(vector<KeyPoint> object_keypoints, vector<KeyPoint> scene_keypoints, vector<DMatch> match_list, float tolerance) {
  double min_distance = DBL_MAX;
  vector<Point2f> object_position, scene_position;
  
  //-- find the minimum distance between matches
  for(size_t i=0; i<match_list.size(); i++) {
    double distance = match_list[i].distance;
    if(distance < min_distance) {
      min_distance = distance;
    }
  }

  //-- find the perspective change using matches that have sufficiently low distance
  for(size_t i=0; i<match_list.size(); i++) {
    double distance = match_list[i].distance;
    if(tolerance == 0 || distance < tolerance*min_distance) {
      object_position.push_back(object_keypoints[match_list[i].queryIdx].pt);
      scene_position.push_back(scene_keypoints[match_list[i].trainIdx].pt);
    }
  }
  if(object_position.size() < 4) { //-- findHomography does not run with less than 4 matches
    return Mat(3,3,CV_32F,Scalar(1));
  }
  return findHomography(object_position, scene_position, CV_RANSAC);
}


//**
//**  SEARCH PATTERN
//**

vector<Pattern_Result> Find_Features::start_search(Mat image) {
  double scene_thickness = estimate_black_line_thickness(image,50,50);

  vector<Pattern_Result> results;
  vector <string> objects;

  //-- add all the potential objects to be searched over to the objects list
  //objects.push_back("feature_samples/wally_from_beach_no_background.png");
  objects.push_back("feature_samples/wally_from_beach_with_background.png");
  objects.push_back("feature_samples/wally_from_shopping_mall_no_background.png");
  //objects.push_back("feature_samples/wally_from_shopping_mall_with_background.png");
  objects.push_back("feature_samples/wally_from_hometown_no_background.png");
  objects.push_back("feature_samples/w_fj_01-02.png");
  objects.push_back("feature_samples/w_fj_03-04.png");
  objects.push_back("feature_samples/w_fj_05-06.png");
  objects.push_back("feature_samples/w_fj_07-08.png");
  objects.push_back("feature_samples/w_fj_09-10.png");
  objects.push_back("feature_samples/w_fj_11-12.png");
  objects.push_back("feature_samples/w_fj_13-14.png");
  objects.push_back("feature_samples/w_fj_15-16.png");
  objects.push_back("feature_samples/w_fj_17-18.png");
  objects.push_back("feature_samples/w_fj_19-20.png");
  objects.push_back("feature_samples/w_fj_21-22.png");
  objects.push_back("feature_samples/w_fj_23-24.png");
  //objects.push_back("feature_samples/wally_large_generic.png");

  vector<Mat> object_list, object_descriptors_list;
  vector<vector<KeyPoint> > object_keypoints_list;
  vector<double> scale_list;
	int largest_object[2]={0,0};
  #pragma omp parallel for default(none) shared(objects, object_list, object_descriptors_list, object_keypoints_list, scale_list, largest_object, scene_thickness)
  for(size_t i=0; i<objects.size(); i++) {

    Mat object = imread(objects[i], CV_LOAD_IMAGE_COLOR);
    Mat image_mask = get_colour_in_image(object, "#FF00FF", "#FF00FF", 1, 0, 1, 1, 0, 1);
		if(largest_object[0] <object.cols) largest_object[0] = object.cols;
		if(largest_object[1] <object.rows) largest_object[1] = object.rows;
    image_mask = image_mask != 255;
    Mat tmp_desc;
    vector<KeyPoint> tmp_kp;
    SIFT sift;
    sift(object, image_mask, tmp_kp, tmp_desc);
    double object_thickness = estimate_black_line_thickness(object,50,50);
    double scale = floor(floor(object_thickness+1)/floor(scene_thickness+1)+0.5);
    #pragma omp critical (findfeatures_keypoints)
    {
      object_descriptors_list.push_back(tmp_desc);
      object_keypoints_list.push_back(tmp_kp);
      object_list.push_back(object);
      scale_list.push_back(scale);
    }
  }
	// calculate best way to split image up amongst processes
  int divisions = omp_get_max_threads();
  int size[2] = {image.cols, image.rows};
  int split[2] = {(int)sqrt(divisions),1};
	while(divisions%split[0] != 0) {
		split[0]--;
	}
	split[1] = divisions/split[0];

	if(size[0]/split[0] < 2*largest_object[0]) {
		split[0] = size[0]/(2*largest_object[0]);
	}
	if(size[1]/split[1] < 2*largest_object[1]) {
		split[1] = size[1]/(2*largest_object[1]);
	}
	if(split[0] < 1) {split[0] = 1;}
	if(split[1] < 1) {split[1] = 1;}
	divisions = split[0]*split[1];

  #pragma omp parallel for default(none) shared(results, image, object_list, object_descriptors_list, object_keypoints_list, scale_list) firstprivate(scene_thickness, divisions, split)
  for(int subimage=0; subimage<divisions; ++subimage) {
    int x[2] = { (subimage/split[0])*(image.rows-1)/split[1], (subimage/split[0]+1)*(image.rows-1)/split[1] };
    int y[2] = { (subimage%split[0])*(image.cols-1)/split[0], (subimage%split[0]+1)*(image.cols-1)/split[0] };

    Mat scene_image = image(Range(x[0],x[1]),Range(y[0],y[1]));

    Mat scene_descriptors;
    vector<KeyPoint> scene_keypoints;
    SIFT sift;
    sift(scene_image, Mat(), scene_keypoints, scene_descriptors);

  	#pragma omp parallel for default(none) shared(results, scene_descriptors, scene_keypoints, object_list, object_descriptors_list, object_keypoints_list, scale_list,x,y)
    for(size_t i=0; i<object_list.size(); i++) {
      Mat object_descriptors = object_descriptors_list[i], object_image = object_list[i];
      vector<KeyPoint> object_keypoints = object_keypoints_list[i];
      double scale = scale_list[i];

      //-- match the  scene and object
      vector<DMatch> match_list = find_matches_with_matcher(object_descriptors, scene_descriptors);
      Mat H = find_homography_within_minimum_distance(object_keypoints, scene_keypoints, match_list,0);

      //-- certainty decided by how close the elements H_{2,0} and H_{2,1} are to their expected value, zero
      double certainty = pow(1-fabs(H.at<double>(2,0)*H.at<double>(2,1))-fabs(H.at<double>(2,0)) -fabs(H.at<double>(2,1)),32);

      //-- defines the box that bounds the scene image match
      vector<Point2f> obj_corners(4), scene_corners(4);
      obj_corners[0] = cvPoint(0,0);
      obj_corners[1] = cvPoint( object_image.cols, 0 );
      obj_corners[2] = cvPoint( object_image.cols, object_image.rows );
      obj_corners[3] = cvPoint( 0, object_image.rows );
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
        #pragma omp critical (findfeatures)
        {
          results.push_back(tmp);
        }
      }
      object_image.release();
    }
    scene_image.release();
  }
  return results;
}
