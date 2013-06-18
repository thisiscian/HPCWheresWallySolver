/*
 *    Part of the serial 'Where's Wally Solver' program.
 *
 *    Defines the Output class, used for outputting and saving data
 *
 *    cian booth - this.is.cian@gmail.com
 *
 */

#ifndef hpc_whereswally_serial_output
#define hpc_whereswally_serial_output

  #include <vector>
  #include "io_variables.h"
  #include "../framework/search_pattern.h"
  
  class Output {
    private:
      IO_Variables *variables;

      // container for results to be displayed
      std::vector<Pattern_Result> final_results;

      // print textual results to the terminal or save to a file
      void show_text_results();
      void save_text_to_file();
  
      // display graphical results or save to a file
      void show_graphic_results();
      void save_graphic_to_file();

      void save_results_to_file();

    public:
      Output(IO_Variables *variables);

      // store results produced by Results_Analysis
      std::vector<Pattern_Result> get_final_results();
      void set_final_results(std::vector<Pattern_Result> results);

      // creates the output as specified by IO_Variables
      void output();
  }; 

#endif
