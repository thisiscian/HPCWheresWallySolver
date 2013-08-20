/*
 * Part of the HPCWheresWallySolver program.
 *
 * Defines the analyser for results, ensures correct number of results comes out, etc
 *
 *   cian booth - this.is.cian@gmail.com
 *
 */
#ifndef hpc_whereswally_serial_results_analysis
#define hpc_whereswally_serial_results_analysis

  #include <set>
  #include <whereswally/framework/search_pattern.h>

  namespace wwp {
    bool compare_results (const Pattern_Result& lhres, const Pattern_Result& rhres);
  };

  class Results_Analysis {
    private:
      std::vector<Pattern_Result> final_results;
    public:
      void calculate_final_results(size_t number_of_final_results, std::vector<Pattern_Result> results);
      void add_to_final_results(std::vector<Pattern_Result> results);
      std::vector<Pattern_Result> get_final_results();
  };

#endif
