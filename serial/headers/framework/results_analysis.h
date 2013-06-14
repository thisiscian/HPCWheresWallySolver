#ifndef hpc_whereswally_serial_results_analysis
#define hpc_whereswally_serial_results_analysis

  #include "search_pattern.h"

  class Results_Analysis {
    public:
      std::vector<Pattern_Result> list_of_results;
      std::vector<Pattern_Result> final_results;
    private:
      void add_results(std::vector<Pattern_Result>);
      void calculate_final_results(int number_of_final_results, std::map<std::string, int> pattern_weighting);
      std::vector<Pattern_Result> get_final_results();
  };

#endif
