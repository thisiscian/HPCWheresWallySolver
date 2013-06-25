#ifndef hpc_whereswally_pattern_red_and_white
#define hpc_whereswally_pattern_red_and_white

  #include <whereswally/framework.h>
  #include <opencv2/imgproc/imgproc.hpp> 

  class Red_and_White : public Search_Pattern {
    private:
      int ratio_to_red;
      int white_threshold;
    public:
      Red_and_White();
      std::vector<Pattern_Result> start_search(cv::Mat image);
  };

#endif
