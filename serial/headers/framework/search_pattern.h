#ifndef hpc_wally_search_pattern
#define hpc_wally_search_pattern

  #include <vector>
  #include "opencv2/core/core.hpp"
  #include "pattern_information.h"

  typedef struct {
    Pattern_Information info;
    int wally_location[2];
    int scale[2];
    float certainty;
  } Pattern_Result;

  class Search_Pattern {
    private:
    public:
      Pattern_Information info;
      Pattern_Information get_pattern_information();
      std::vector<Pattern_Result> start_search(cv::Mat image);
  };

#endif
