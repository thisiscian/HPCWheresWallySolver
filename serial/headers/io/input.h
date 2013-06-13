#ifndef hpc_wally_input
#define hpc_wally_input

  #include <iostream>
  #include <opencv2/core/core.hpp>
  #include "io_variables.h"

  enum input_errors {
    NO_ERRORS,
    UNDEFINED,
    PRINT_HELP,
    BAD_CONFIG_FILE,
    CONTRADICTING_INPUTS
  };

  class Input {
    private:
    public:
      IO_Variables* variables;

      int parse_input(int argc, char* argv[]);
      int load_config();
      int load_results();
      void set_variables(IO_Variables new_variables);
      cv::Mat load_image();

      Input(IO_Variables *variables);
  };

#endif
