#ifndef hpc_whereswally_serial_unittest_error_message
#define hpc_whereswally_serial_unittest_error_message

  #include <string>
  #include <iostream>
  #include <sstream>
  #include <cppunit/extensions/HelperMacros.h>
  #include "../../../headers/io/error_message.h"
  #include "../general_test_functions.h"

  class error_message_test : public CppUnit::TestFixture {

    CPPUNIT_TEST_SUITE(error_message_test);
      CPPUNIT_TEST(test_number_of_final_results);
      CPPUNIT_TEST(test_pattern_weighting);
      CPPUNIT_TEST(test_config_input_filename);
      CPPUNIT_TEST(test_puzzle_input_filename);
      CPPUNIT_TEST(test_results_input_filename);
      CPPUNIT_TEST(test_text_output_filename);
      CPPUNIT_TEST(test_graphic_output_filename);
      CPPUNIT_TEST(test_results_output_filename);
    
    CPPUNIT_TEST_SUITE_END();
    public:
      void setUp();
      void tearDown();

      void test_number_of_final_results();
      void test_pattern_weighting();

      void test_config_input_filename();
      void test_puzzle_input_filename();
      void test_results_input_filename();

      void test_text_output_filename();
      void test_graphic_output_filename();
      void test_results_output_filename();
    
      void test_false_error();
    private:
      Error_Message err_msg;
      IO_Monitor monitor;
  };

#endif
