#ifndef hpc_whereswally_serial_unittest_input
#define hpc_whereswally_serial_unittest_input

  #include <string>
  #include <iostream>
  #include <sstream>
  #include <cppunit/extensions/HelperMacros.h>
  #include "../../../headers/io/input.h"
  #include "../general_test_functions.h"

  class input_test : public CppUnit::TestFixture {

    CPPUNIT_TEST_SUITE(input_test);
      CPPUNIT_TEST(test_no_arguments);
      CPPUNIT_TEST(test_load_config);
      CPPUNIT_TEST(test_load_results);
      CPPUNIT_TEST(test_option_config);
      CPPUNIT_TEST(test_option_number_of_final_results);
      CPPUNIT_TEST(test_option_pattern_weighting);
      CPPUNIT_TEST(test_option_solve_puzzle);
      CPPUNIT_TEST(test_option_load_results);
      CPPUNIT_TEST(test_option_graphic_output);
      CPPUNIT_TEST(test_option_text_output);
      CPPUNIT_TEST(test_option_save_graphic_output);
      CPPUNIT_TEST(test_option_save_text_output);
      CPPUNIT_TEST(test_option_help);
      CPPUNIT_TEST(test_unrecognised_command);
      CPPUNIT_TEST(test_constructor);
    CPPUNIT_TEST_SUITE_END();
    public:
      void setUp();
      void tearDown();

      void test_constructor();
      void test_load_config();
      void test_load_results();
      
      void test_option_config();
      void test_option_number_of_final_results();
      void test_option_pattern_weighting();
      void test_option_solve_puzzle();
      void test_option_load_results();
      void test_option_graphic_output();
      void test_option_text_output();
      void test_option_save_graphic_output();
      void test_option_save_text_output();
      void test_option_help();

      void test_no_arguments();
      void test_unrecognised_command();
    private:
      Error_Message err_msg;
      IO_Variables *var;
      Input *input;
      IO_Monitor monitor;
  };

#endif
