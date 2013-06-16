#ifndef hpc_whereswally_serial_unittest_output
#define hpc_whereswally_serial_unittest_output

  #include <string>
  #include <iostream>
  #include <sstream>
  #include <cppunit/extensions/HelperMacros.h>
  #include "../../../headers/io/output.h"
  #include "../general_test_functions.h"

  class output_test : public CppUnit::TestFixture {

    CPPUNIT_TEST_SUITE(output_test);
      CPPUNIT_TEST(test_constructor);
    CPPUNIT_TEST_SUITE_END();
    public:
      void setUp();
      void tearDown();

      void test_constructor();
    private:
      IO_Variables *var;
      Output *output;
      IO_Monitor monitor;
  };

#endif
