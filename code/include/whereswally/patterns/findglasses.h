/*
 * Part of the HPCWheresWallySolver program.
 *
 * Find Wally with his glasses
 *
 *   cian booth - this.is.cian@gmail.com
 *
 */
#ifndef hpc_whereswally_pattern_find_glasses
#define hpc_whereswally_pattern_find_glasses

  #include <whereswally/framework.h>
  #include "opencv2/imgproc/imgproc.hpp"
  
  class Find_Glasses : public Search_Pattern {
    private:
      double rescale;
      double allowed_irregularity;
      int thresholds;
    public:
    Find_Glasses();
    wwp::region extract_contour_info(std::vector<cv::Point>);
    double calculate_angle(cv::Point p1, cv::Point p2, cv::Point p3);
    std::vector<Pattern_Result> start_search(cv::Mat image);
  };

#endif

