#ifndef hpc_whereswally_serial_unittest_io_variables
#define hpc_whereswally_serial_unittest_io_variables

  #include <string>
  #include <iostream>
  #include <sstream>
  #include <cppunit/extensions/HelperMacros.h>
  #include <cppunit/CompilerOutputter.h>
  #include <cppunit/extensions/TestFactoryRegistry.h>
  #include <cppunit/ui/text/TestRunner.h>
  #include "whereswally/io/io_variables.h"
  #include "../general_test_functions.h"

  class io_variables_test : public CppUnit::TestFixture {

    // add functions to test suite
    CPPUNIT_TEST_SUITE(io_variables_test);
      CPPUNIT_TEST(test_load_config_from_file);
      CPPUNIT_TEST(test_config_input_filename);

      CPPUNIT_TEST(test_load_puzzle_from_file);
      CPPUNIT_TEST(test_puzzle_input_filename);

      CPPUNIT_TEST(test_load_results_from_file);
      CPPUNIT_TEST(test_results_input_filename);
      CPPUNIT_TEST(test_loaded_results);

      CPPUNIT_TEST(test_number_of_final_results);

      CPPUNIT_TEST(test_pattern_weighting);

      CPPUNIT_TEST(test_show_text_results);
      CPPUNIT_TEST(test_make_text_verbose);
      CPPUNIT_TEST(test_save_text_to_file);
      CPPUNIT_TEST(test_text_output_filename);

      CPPUNIT_TEST(test_show_graphic_results);
      CPPUNIT_TEST(test_make_graphic_verbose);
      CPPUNIT_TEST(test_save_graphic_to_file);
      CPPUNIT_TEST(test_graphic_output_filename);

      CPPUNIT_TEST(test_save_results_to_file);
      CPPUNIT_TEST(test_results_output_filename);

      CPPUNIT_TEST(test_loaded_image);

      CPPUNIT_TEST(test_constructor);
    CPPUNIT_TEST_SUITE_END();
    public:
      // construction and deconstruction before tests
      void setUp();
      void tearDown();

      // test get,set and behaviour for IO_Variables 
      void test_load_config_from_file();
      void test_config_input_filename();

      void test_load_puzzle_from_file();
      void test_puzzle_input_filename();
      void test_loaded_image();

      void test_load_results_from_file();
      void test_results_input_filename();
      void test_loaded_results();

      void test_number_of_final_results();

      void test_pattern_weighting();

      void test_show_text_results();
      void test_make_text_verbose();
      void test_save_text_to_file();
      void test_text_output_filename();

      void test_show_graphic_results();
      void test_make_graphic_verbose();
      void test_save_graphic_to_file();
      void test_graphic_output_filename();

      void test_save_results_to_file();
      void test_results_output_filename();
      
      // test construction
      void test_constructor();
    private:
      IO_Variables *tmp_var; // IO_Variables class that gets rebuilt for each test
      IO_Monitor monitor;  // allows test to silence & record stdout and stderr
      Error_Message err_msg;
  };

#endif
