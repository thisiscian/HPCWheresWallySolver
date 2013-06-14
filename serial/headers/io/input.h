/*
 *    Part of the serial 'Where's Wally Solver' program.
 *
 *    Defines the Input class, used for parsing user input and loading files.
 *    Allows program to parse user input via argc and argv[].
 *    Stores parsed input into an IO_Variables class (io_variables.h).
 *    Can also load configuration for the IO_Variables class and load results
 *      from a previous session.
 *
 *    cian booth - this.is.cian@gmail.com
 *
 */

#ifndef hpc_whereswally_serial_input
#define hpc_whereswally_serial_input

  #include <iostream>
  #include <opencv2/core/core.hpp>
  #include "io_variables.h"

  enum input_errors {
    NO_ERRORS,
    UNDEFINED,
    BAD_CONFIG_FILE,
    BAD_RESULTS_FILE,
    CONTRADICTING_INPUTS
  };

  class Input {
    private:
    public:
      Input(IO_Variables *variables);
      IO_Variables* variables;

      // parse input from argc and argv[]
      int parse_input(int argc, char* argv[]);
      
      // load values for IO_Variables from a configuration file
      int load_config();
      
      // load results from a file (as defined in IO_Variables.get_load_results_filename), which are to be analysed
      int load_results();

      // load a puzzle image from a file (as defined in IO_Variables.get_puzzle_image_filename), which is to be solved
      cv::Mat load_image();
  };

#endif
