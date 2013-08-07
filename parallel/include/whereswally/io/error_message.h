#ifndef hpc_whereswally_serial_error_messages
#define hpc_whereswally_serial_error_messages

#include <iostream>
#include <sstream>
#include <string>
#include <cstdarg>

enum errors {
  NO_ERRORS,
  NO_ERRORS_QUIT,
  NEGATIVE_CONFIDENCE,
  CONFIG_FILE_DOES_NOT_EXIST,
  UNDEFINED,
  NO_CONFIG_FILE,
  BAD_CONFIG_FILE,
  NO_RESULTS_FILE,
  BAD_RESULTS_FILE,
  CONTRADICTING_INPUTS,
  UNRECOGNISED_COMMAND,
  BAD_COMMAND_USAGE
};

class Error_Message {
  private:
    std::string bad_number_of_final_results(va_list vl);
    std::string bad_number_of_openmp_threads(va_list vl);
    std::string bad_pattern_weighting(va_list vl);

    std::string bad_config_input_filename(va_list vl);
    std::string bad_puzzle_input_filename(va_list vl);
    std::string bad_results_input_filename(va_list vl);

    std::string bad_text_output_filename(va_list vl);
    std::string bad_graphic_output_filename(va_list vl);
    std::string bad_results_output_filename(va_list vl);

    std::string bad_option_config(va_list vl);
    std::string bad_option_number_of_openmp_threads(va_list vl);
    std::string bad_option_number_of_final_results(va_list vl);
    std::string bad_option_pattern_weighting(va_list vl);
    std::string bad_option_load_puzzle(va_list vl);
    std::string bad_option_load_results(va_list vl);
    std::string bad_option_show_text(va_list vl);
    std::string bad_option_show_graphic(va_list vl);
    std::string bad_option_show_timing_results(va_list vl);
    std::string bad_option_save_text(va_list vl);
    std::string bad_option_save_graphic(va_list vl);
    std::string bad_option_save_results(va_list vl);
    std::string bad_option_help(va_list vl);

    std::string unrecognised_error(std::string bad_error);
  public:
    std::string operator()(std::string,...);
};

#endif
