#ifndef hpc_whereswally_serial_unittest_search_pattern
#define hpc_whereswally_serial_unittest_search_pattern

  #include <string>
  #include <iostream>
  #include <sstream>
  #include <cppunit/extensions/HelperMacros.h>
  #include "../../../headers/framework/search_pattern.h"
  #include "../general_test_functions.h"

  class search_pattern_test : public CppUnit::TestFixture {
    // add functions to test suite
    CPPUNIT_TEST_SUITE(search_pattern_test);
      CPPUNIT_TEST(test_pattern_information);
      CPPUNIT_TEST(test_result_relational_operators);
    CPPUNIT_TEST_SUITE_END();
    public:
      // construction and deconstruction before tests
      void setUp();
      void tearDown();

      // test various get,sets of values
      void test_pattern_information();

      // test relational operators == and != for Pattern_Result
      void test_result_relational_operators();
    private:
      Search_Pattern *tmp_search;
      Pattern_Result result;
      Record_Output record_output;  // allows test to silence & record stdout and stderr
  };

#endif
