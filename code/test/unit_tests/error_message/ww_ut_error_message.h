#ifndef hpc_whereswally_serial_unittest_error_message
#define hpc_whereswally_serial_unittest_error_message

  #include <string>
  #include <iostream>
  #include <sstream>
  #include <cppunit/extensions/HelperMacros.h>
  #include "whereswally/io/error_message.h"
  #include "../general_test_functions.h"

  class error_message_test : public CppUnit::TestFixture {

    CPPUNIT_TEST_SUITE(error_message_test);
      CPPUNIT_TEST(test_number_of_final_results);
      CPPUNIT_TEST(test_puzzle_input_filename);
      CPPUNIT_TEST(test_option_number_of_results);
      CPPUNIT_TEST(test_option_load_puzzle);
      CPPUNIT_TEST(test_option_show_text);
      CPPUNIT_TEST(test_option_show_graphic);
      CPPUNIT_TEST(test_option_help); 
    
    CPPUNIT_TEST_SUITE_END();
    public:
      void setUp();
      void tearDown();

      void test_number_of_final_results();

      void test_puzzle_input_filename();

      void test_option_number_of_results();
      void test_option_load_puzzle();
      void test_option_show_text();
      void test_option_show_graphic();
      void test_option_help();

      void test_false_error();
    private:
      Error_Message err_msg;
      IO_Monitor monitor;
  };

#endif
