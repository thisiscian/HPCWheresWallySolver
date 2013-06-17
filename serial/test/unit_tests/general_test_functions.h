#ifndef hpc_whereswally_serial_unittest_general
#define hpc_whereswally_serial_unittest_general
  #include <iostream>
  #include <sstream>
  #include <string>

  class IO_Monitor {
    public:
      void start_recording_cout();
      void stop_recording_cout();
      std::string get_cout_output();
      void start_recording_cerr();
      void stop_recording_cerr();
      std::string get_cerr_output();
      void silence(), unsilence();
      IO_Monitor();
    private:
      bool silent;
      std::stringstream cout_buffer,cerr_buffer;
      std::streambuf *old_cout, *old_cerr;
      std::string cout_output, cerr_output;
  };
#endif

