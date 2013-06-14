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

      // 'print' textual results to the terminal or a file
      void print_results();
      void save_print_to_file();
  
      // 'display' graphical results in a new window or save to a file
      void display_results();
      void save_display_to_file();
    public:
      Output(IO_Variables *variables);

      // creates the output as specified by IO_Variables
      void output();
  }; 

#endif
