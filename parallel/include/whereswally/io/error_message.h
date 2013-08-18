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

#ifndef hpc_whereswally_serial_error_messages
#define hpc_whereswally_serial_error_messages

  #include <iostream>
  #include <sstream>
  #include <string>
  #include <cstdarg>

  //-- error return values
  enum errors {
    NO_ERRORS,
    NO_ERRORS_QUIT,
    NEGATIVE_CONFIDENCE,
    CONFIG_FILE_DOES_NOT_EXIST,
    UNDEFINED,
    NO_CONFIG_FILE,
    BAD_CONFIG_FILE,
    UNRECOGNISED_COMMAND,
    BAD_COMMAND_USAGE
  };

  //-- Class for outputting error messages
  class Error_Message {
    private:
      std::string bad_number_of_final_results(va_list vl);
      std::string bad_number_of_openmp_threads(va_list vl);

      std::string bad_config_input_filename(va_list vl);
      std::string bad_puzzle_input_filename(va_list vl);
      std::string bad_results_input_filename(va_list vl);

      std::string bad_option_config(va_list vl);
      std::string bad_option_number_of_openmp_threads(va_list vl);
      std::string bad_option_number_of_final_results(va_list vl);
      std::string bad_option_load_puzzle(va_list vl);
      std::string bad_option_load_results(va_list vl);
      std::string bad_option_show_text(va_list vl);
      std::string bad_option_show_graphic(va_list vl);
      std::string bad_option_show_timing(va_list vl);
      std::string bad_option_help(va_list vl);

      std::string unrecognised_error(std::string bad_error);
    public:
      std::string operator()(std::string,...);
  };

#endif
