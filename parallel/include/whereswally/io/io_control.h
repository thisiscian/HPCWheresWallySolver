/*
 *    Part of the serial 'Where's Wally Solver' program.
 *
 *    Defines the IO_Control class, used for control over all I/O operations.
 *    Contains the IO_Variables class (io_variables.h) that allows the Input
 *      class (input.h), the Output class (output.h) and the Result_Analysis
 *      class (result_analysis.h) to interact.
 *
 *    cian booth - this.is.cian@gmail.com
 *
 */


#ifndef hpc_whereswally_serial_io_control
#define hpc_whereswally_serial_io_control

  #include <whereswally/framework.h>
  #include <whereswally/io/io_variables.h>
  #include <whereswally/io/input.h>
  #include <whereswally/io/output.h>
  #include <omp.h>
  
  class IO_Control {
    private:
      IO_Variables *variables;

      void load_patterns_dynamically();
    public:
      IO_Control(int argc, char* argv[]);
      Input *input;
      Output *output;
      Results_Analysis analysis;

      // needed for analysis of results by Global_Results (global_results.h)
      int start(std::vector<Search_Pattern*>);
  };

#endif
