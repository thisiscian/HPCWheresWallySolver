/*
 * Part of the HPCWheresWallySolver program.
 *
 * Find Wally by his blue trousers
 *
 *   cian booth - this.is.cian@gmail.com
 *
 */
#ifndef hpc_whereswally_pattern_blue_trousers
#define hpc_whereswally_pattern_blue_trousers

  #include <whereswally/framework.h>
  #include <opencv2/imgproc/imgproc.hpp> 

  class Blue_Trousers : public Search_Pattern {
    public:
      Blue_Trousers();
      Blue_Trousers(Search_Pattern *next, float next_certainty);
      std::vector<Pattern_Result> start_search(cv::Mat image);
  };

#endif
