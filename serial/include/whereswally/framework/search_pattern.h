#ifndef hpc_whereswally_serial_search_pattern
#define hpc_whereswally_serial_search_pattern

  #include <vector>
  #include <opencv2/core/core.hpp>
  #include <whereswally/framework/pattern_information.h>

  typedef struct {
    Pattern_Information info;
    int wally_location[2];
    int scale[2];
    float certainty;
  } Pattern_Result;

  const bool operator==(const Pattern_Result &lhs, const Pattern_Result &rhs);
  const bool operator>(const Pattern_Result &lhs, const Pattern_Result &rhs);
  const bool operator!=(const Pattern_Result &lhs, const Pattern_Result &rhs);

  class Search_Pattern {
    private:
    public:
      Pattern_Information info;
      const Pattern_Information get_pattern_information();
      virtual std::vector<Pattern_Result> start_search(cv::Mat image);
  };

#endif
