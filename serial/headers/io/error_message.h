#ifndef hpc_whereswally_serial_error_messages
#define hpc_whereswally_serial_error_messages

#include <iostream>
#include <sstream>
#include <string>
#include <cstdarg>

class Error_Message {
  private:
    std::string bad_number_of_final_results(va_list vl);
    std::string bad_pattern_weighting(va_list vl);

    std::string bad_config_input_filename(va_list vl);
    std::string bad_puzzle_input_filename(va_list vl);
    std::string bad_results_input_filename(va_list vl);

    std::string bad_text_output_filename(va_list vl);
    std::string bad_graphic_output_filename(va_list vl);
    std::string bad_results_output_filename(va_list vl);

    std::string unrecognised_error(std::string bad_error);
  public:
    std::string operator()(std::string,...);
};

#endif
