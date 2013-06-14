/*
 *    Part of the serial 'Where's Wally Solver' program.
 *
 *    Defines the IO_Control class, used for control over all I/O operations.
 *    Contains the IO_Variables class (io_variables.h) that allows the Input
 *      class (input.h) and the Output class (output.h) to interact.
 *
 *    cian booth - this.is.cian@gmail.com
 *
 */


#ifndef hpc_whereswally_serial_io_control
#define hpc_whereswally_serial_io_control

  #include "io_variables.h"
  #include "input.h"
  #include "output.h"
  
  class IO_Control {
    private:
      IO_Variables *variables;
    public:
      IO_Control();
      Input *input;
      Output *output;

      // needed for analysis of results by Global_Results (global_results.h)
      int get_number_of_final_results();
      int get_weighting(std::string pattern_name);
  };

#endif
