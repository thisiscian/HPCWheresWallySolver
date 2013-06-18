#ifndef hpc_whereswally_serial_results_analysis
#define hpc_whereswally_serial_results_analysis

  #include "search_pattern.h"
  #include <set>

  class Results_Analysis {
    private:
      std::vector<Pattern_Result> final_results;
    public:
      void calculate_final_results(int number_of_final_results, std::vector<Pattern_Result> results);
      void add_to_final_results(std::vector<Pattern_Result> results);
      std::vector<Pattern_Result> get_final_results();
  };

#endif
