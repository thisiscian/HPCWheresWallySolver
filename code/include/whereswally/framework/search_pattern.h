/*
 * Part of the HPCWheresWallySolver program.
 *
 * Defines the class that contains information on results that have been calculated
 * Also defines the class that users extend to define their own pattern
 *
 *   cian booth - this.is.cian@gmail.com
 *
 */
#ifndef hpc_whereswally_serial_search_pattern
#define hpc_whereswally_serial_search_pattern

  #include <vector>
  #include <opencv2/core/core.hpp>
  #include <whereswally/framework/pattern_information.h>

  class Pattern_Result {
    public:
    Pattern_Information info;
    int wally_location[2];
    int scale[2];
    float certainty;
  };

  const bool operator==(const Pattern_Result &lhs, const Pattern_Result &rhs);
  const bool operator>(const Pattern_Result &lhs, const Pattern_Result &rhs);
  const bool operator!=(const Pattern_Result &lhs, const Pattern_Result &rhs);

  class Search_Pattern {
    private:
    public:
      Pattern_Information info;
      const Pattern_Information get_pattern_information();
      virtual std::vector<Pattern_Result> start_search(cv::Mat image);
      Search_Pattern *next;
      float next_certainty;
  };

#endif
