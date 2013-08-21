/*
 * Part of the HPCWheresWallySolver program.
 *
 * Defines the Input class, used for parsing user input and loading files.
 * Allows program to parse user input via argc and argv[].
 * Stores parsed input into an IO_Variables class (io_variables.h).
 * Can also load configuration for the IO_Variables class and load results
 *   from a previous session.
 *
 *   cian booth - this.is.cian@gmail.com
 *
 */

#ifndef hpc_whereswally_serial_input
#define hpc_whereswally_serial_input

  #include <iostream>
  #include <fstream>
  #include <algorithm>
  #include <cctype>
  #include <cstdlib>
  #include <opencv2/core/core.hpp>
  #include <opencv2/highgui/highgui.hpp>
  #include <whereswally/io/io_variables.h>

  namespace wwp {
      //** HELPER FUNCTIONS
      int to_lower_case(int c);
      std::string remove_whitespace(std::string in_string);
      std::string get_option_variable(int *num, int argc, char *argv[]);
      std::string get_option_flag(int num, char *argv[]);
  }

  class Input {
    private:
      Error_Message err_msg;
        
      //** INPUT FUNCTIONS
      int option_config(int *num, int argc, char *argv[]);
      int option_openmp(int *num, int argc, char *argv[]);
      int option_number_of_final_results(int *num, int argc, char *argv[]);
      int option_number_of_openmp_threads(int *num, int argc, char *argv[]);
      int option_load_puzzle(int *num, int argc, char *argv[]);
      int option_show_text(int *num, int argc, char *argv[]);
      int option_show_graphic(int *num, int argc, char *argv[]);
      int option_show_timing(int *num, int argc, char *argv[]);
      int option_help(int *num, int argc, char *argv[]);
    public:
      Input(IO_Variables *variables);
      IO_Variables* variables;

      int parse_input(int argc, char* argv[]); //-- parses user input
      int load_config(); //-- loads values into *variables from a configuration file
      int load_results(); //-- loads previously calculated results into *variables
      void load_image(); //-- loads a puzzle image from a file, as defined in variables->get_puzzle_image_filename
  };

#endif
