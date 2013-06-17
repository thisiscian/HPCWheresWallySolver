#ifndef hpc_whereswally_serial_unittest_io_control
#define hpc_whereswally_serial_unittest_io_control

  #include <string>
  #include <iostream>
  #include <sstream>
  #include <cppunit/extensions/HelperMacros.h>
  #include "../../../headers/io/io_control.h"
  #include "../general_test_functions.h"

  class io_control_test : public CppUnit::TestFixture {

    CPPUNIT_TEST_SUITE(io_control_test);
    CPPUNIT_TEST_SUITE_END();
    public:
      void setUp();
      void tearDown();
    private:
      IO_Monitor monitor;
  };

#endif
