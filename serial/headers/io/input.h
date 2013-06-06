#ifndef hpc_wally_input
#define hpc_wally_input

  #include <opencv2/core/core.hpp>
  #include "io_variables.h"

  class Input {
    private:
      IO_Variables variables;
    public:
      int parse_input(int argc, char* argv[]);
      int load_config();
      int load_results();
      void set_variables(IO_Variables new_variables);
      IO_Variables get_variables();
      Mat load_image();
  };

#endif
