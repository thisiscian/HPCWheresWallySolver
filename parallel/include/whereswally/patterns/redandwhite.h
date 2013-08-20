/*
 * Part of the HPCWheresWallySolver program.
 *
 * Find Wally by his stripes
 *
 *   cian booth - this.is.cian@gmail.com
 *
 */

#ifndef hpc_whereswally_pattern_red_and_white
#define hpc_whereswally_pattern_red_and_white

  #include <whereswally/framework.h>
  #include <opencv2/imgproc/imgproc.hpp> 

  class Red_and_White : public Search_Pattern {
    public:
      Red_and_White();
      std::vector<Pattern_Result> start_search(cv::Mat image);
  };

#endif
