#ifndef hpc_whereswally_pattern_find_features
#define hpc_whereswally_pattern_find_features

  #include <whereswally/framework.h>
  #include "opencv2/imgproc/imgproc.hpp"
  #include "opencv2/features2d/features2d.hpp"
  #include "opencv2/calib3d/calib3d.hpp"
  #include "opencv2/nonfree/features2d.hpp" //?
  
  class Find_Features : public Search_Pattern {
    private:
    int minHessian;
    int knn_depth;
    public:
    Find_Features();
    std::vector<cv::KeyPoint> get_keypoints_from_image(cv::Mat image);
    std::vector<Pattern_Result> start_search(cv::Mat image);
  };

#endif

