#ifndef hpc_wally_output
#define hpc_wally_output

  #include <vector>
  #include "io_variables.h"
  #include "../framework/search_pattern.h"
  
  class Output {
    private:
      IO_Variables *variables;
      std::vector<Pattern_Result> final_results;
      void print_results(), display_results();
      void save_print_to_file();
    public:
      int get_number_of_final_results();
      int get_weighting(std::string pattern_name);
      void output();

      Output(IO_Variables *variables);
  }; 

#endif
