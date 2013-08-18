#ifndef hpc_whereswally_pattern_find_features
#define hpc_whereswally_pattern_find_features

  #include <whereswally/framework.h>
  #include "opencv2/imgproc/imgproc.hpp"
  #include "opencv2/features2d/features2d.hpp"
  #include "opencv2/calib3d/calib3d.hpp"
  #include "opencv2/nonfree/features2d.hpp" //?
  
  class Find_Features : public Search_Pattern {
    private:
    //helper functions
    double dist(cv::Point2f pt1, cv::Point2f pt2);
    double dist(cv::Point2f pt);
    int count_order(std::vector<double> list);
    int compare_rows(float x[], float y[], int x_len, int y_len);
    double homography_quality(wwp::homography H);
    wwp::homography calculate_scale_and_translation(std::vector< std::vector<cv::DMatch > > matches, std::vector<cv::KeyPoint> object_keypoints, std::vector<cv::KeyPoint> scene_keypoints, int i, int j, int k, int l);

    //methods to get matches
    std::vector<cv::DMatch> find_matches_with_equivalent_distances(std::vector<cv::KeyPoint> object_keypoints,std::vector<cv::KeyPoint> scene_keypoints, cv::Mat object_descriptors, cv::Mat scene_descriptors);
    std::vector<cv::DMatch> find_matches_with_equivalent_ratios(std::vector<cv::KeyPoint> object_keypoints,std::vector<cv::KeyPoint> scene_keypoints, cv::Mat object_descriptors, cv::Mat scene_descriptors);
    std::vector<cv::DMatch> find_matches_with_matcher(cv::Mat object_descriptors, cv::Mat scene_descriptors);
    std::vector<cv::DMatch> find_symmetric_matches_with_matcher(cv::Mat object_descriptors, cv::Mat scene_descriptors);
    std::vector< std::vector<cv::DMatch> > find_n_degree_matches_with_matcher(cv::Mat object_descriptors, cv::Mat scene_descriptors, int n);

    //methods to find perspective change between matches
    cv::Mat find_homography_within_minimum_distance(std::vector<cv::KeyPoint> object_keypoints, std::vector<cv::KeyPoint> scene_keypoints, std::vector<cv::DMatch> match_list, float tolerance);
    cv::Mat find_homography_with_translational_invariance(std::vector<cv::KeyPoint> object_keypoints, std::vector<cv::KeyPoint> scene_keypoints, std::vector< std::vector <cv::DMatch> > match_list);
    public:
    Find_Features();
    std::vector<Pattern_Result> start_search(cv::Mat image);
  };

#endif

