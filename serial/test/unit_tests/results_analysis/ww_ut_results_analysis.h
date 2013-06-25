#ifndef hpc_whereswally_serial_unittest_results_analysis
#define hpc_whereswally_serial_unittest_results_analysis

  #include <string>
  #include <iostream>
  #include <sstream>
  #include <cppunit/extensions/HelperMacros.h>
  #include "whereswally/framework/results_analysis.h"
  #include "../general_test_functions.h"

  class results_analysis_test : public CppUnit::TestFixture {
    // add functions to test suite
    CPPUNIT_TEST_SUITE(results_analysis_test);
      CPPUNIT_TEST(test_calculate_final_results);
      CPPUNIT_TEST(test_final_results);
    CPPUNIT_TEST_SUITE_END();
    public:
      // construction and deconstruction before tests
      void setUp();
      void tearDown();
    
      void test_calculate_final_results();
      void test_final_results();

    private:
      std::vector<Pattern_Result> input_results;
      std::vector<Pattern_Result> output_results;
      Results_Analysis *ra;
      IO_Monitor monitor;  // allows test to silence & record stdout and stderr
  };

#endif
