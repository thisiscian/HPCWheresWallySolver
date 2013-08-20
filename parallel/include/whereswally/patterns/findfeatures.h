/*
 * Part of the HPCWheresWallySolver program.
 *
 * Find Wally by matching other pictures of him
 *
 *   cian booth - this.is.cian@gmail.com
 *
 */
#ifndef hpc_whereswally_pattern_find_features
#define hpc_whereswally_pattern_find_features

  #include <whereswally/framework.h>
  #include "opencv2/imgproc/imgproc.hpp"
  #include "opencv2/features2d/features2d.hpp"
  #include "opencv2/calib3d/calib3d.hpp"
  #include "opencv2/nonfree/features2d.hpp" 
  #include <omp.h>
  #include <map>
  #include <cfloat>
  #include <algorithm>
  #include <cmath>
  #include "opencv2/highgui/highgui.hpp"
 
  class Find_Features : public Search_Pattern {
    private:
    //-- helper functions
    double dist(cv::Point2f pt1, cv::Point2f pt2);
    double dist(cv::Point2f pt);
    int count_order(std::vector<double> list);
    int compare_rows(float x[], float y[], int x_len, int y_len);

    //-- methods to get matches
    std::vector<cv::DMatch> find_matches_with_equivalent_distances(std::vector<cv::KeyPoint> object_keypoints,std::vector<cv::KeyPoint> scene_keypoints, cv::Mat object_descriptors, cv::Mat scene_descriptors);
    std::vector<cv::DMatch> find_matches_with_equivalent_ratios(std::vector<cv::KeyPoint> object_keypoints,std::vector<cv::KeyPoint> scene_keypoints, cv::Mat object_descriptors, cv::Mat scene_descriptors);
    std::vector<cv::DMatch> find_matches_with_matcher(cv::Mat object_descriptors, cv::Mat scene_descriptors);
    std::vector<cv::DMatch> find_symmetric_matches_with_matcher(cv::Mat object_descriptors, cv::Mat scene_descriptors);
    std::vector< std::vector<cv::DMatch> > find_n_degree_matches_with_matcher(cv::Mat object_descriptors, cv::Mat scene_descriptors, int n);

    //methods to find perspective change between matches
    cv::Mat find_homography_within_minimum_distance(std::vector<cv::KeyPoint> object_keypoints, std::vector<cv::KeyPoint> scene_keypoints, std::vector<cv::DMatch> match_list, float tolerance);
    public:
    Find_Features();
    std::vector<Pattern_Result> start_search(cv::Mat image);
  };

#endif

