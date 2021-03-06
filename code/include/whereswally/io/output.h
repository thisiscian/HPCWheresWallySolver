/*
 * Part of the HPCWheresWallySolver program.
 *
 * Defines the Output class, used for outputting data
 *
 *   cian booth - this.is.cian@gmail.com
 *
 */

#ifndef hpc_whereswally_serial_output
#define hpc_whereswally_serial_output

  #include <iostream>
  #include <sstream>
  #include <string>
  #include <vector>
  #include <opencv2/highgui/highgui.hpp>
  #include <whereswally/framework.h>
  #include <whereswally/io/io_variables.h>
  
  class Output {
    private:
      IO_Variables *variables;

      // container for results to be displayed
      std::vector<Pattern_Result> final_results;

      // print textual results to the terminal or save to a file
      void show_text_results();
  
      // display graphical results or save to a file
      void show_graphic_results();

      void show_timing_results();


    public:
      Output(IO_Variables *variables);

      // store results produced by Results_Analysis
      std::vector<Pattern_Result> get_final_results();
      void set_final_results(std::vector<Pattern_Result> results);

      // creates the output as specified by IO_Variables
      void output();
  }; 

#endif
