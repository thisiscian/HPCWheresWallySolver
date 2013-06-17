#ifndef hpc_whereswally_serial_unittest_pattern_information
#define hpc_whereswally_serial_unittest_pattern_information

  #include <string>
  #include <iostream>
  #include <sstream>
  #include <cppunit/extensions/HelperMacros.h>
  #include "../../../headers/framework/pattern_information.h"
  #include "../general_test_functions.h"

  class pattern_information_test : public CppUnit::TestFixture {

    // add functions to test suite
    CPPUNIT_TEST_SUITE(pattern_information_test);
      CPPUNIT_TEST(test_name);
      CPPUNIT_TEST(test_description);
      CPPUNIT_TEST(test_confidence);
      CPPUNIT_TEST(test_relational_operators);
      CPPUNIT_TEST(test_constructor);
    CPPUNIT_TEST_SUITE_END();
    public:
      // construction and deconstruction before tests
      void setUp();
      void tearDown();

      // test various get,sets of values
      void test_name();
      void test_description();
      void test_confidence();

      // test relational operators == and !=
      void test_relational_operators();

      // test construction
      void test_constructor();
    private:
      Pattern_Information *tmp_patt;
      IO_Monitor monitor;  // allows test to silence & record stdout and stderr
  };

#endif
