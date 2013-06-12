#ifndef hpc_wally_io_control

  #include "io_variables.h"
  #include "input.h"
  #include "output.h"
  
  class IO_Control {
    private:
      IO_Variables variables;
    public:
      Input *input;
      Output *output;
      IO_Control();
  };

#endif
