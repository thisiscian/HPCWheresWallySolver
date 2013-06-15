#ifndef hpc_whereswally_serial_unittest_io_variables
#define hpc_whereswally_serial_unittest_io_variables

  #include <string>
  #include <iostream>
  #include <sstream>
  #include <cppunit/extensions/HelperMacros.h>
  #include "../../../headers/io/io_variables.h"
  #include "../general_test_functions.h"

  class io_variables_test : public CppUnit::TestFixture {

    // add functions to test suite
    CPPUNIT_TEST_SUITE(io_variables_test);
      CPPUNIT_TEST(test_number_of_final_results);
      CPPUNIT_TEST(test_pattern_weighting);
      CPPUNIT_TEST(test_print_results);
      CPPUNIT_TEST(test_display_results);
      CPPUNIT_TEST(test_verbose_print);
      CPPUNIT_TEST(test_verbose_display);
      CPPUNIT_TEST(test_save_print_to_file);
      CPPUNIT_TEST(test_save_display_to_file);
      CPPUNIT_TEST(test_load_puzzle_from_file);
      CPPUNIT_TEST(test_load_config_from_file);
      CPPUNIT_TEST(test_load_results_from_file);
      CPPUNIT_TEST(test_print_output_filename);
      CPPUNIT_TEST(test_display_output_filename);
      CPPUNIT_TEST(test_puzzle_image_filename);
      CPPUNIT_TEST(test_config_filename);
      CPPUNIT_TEST(test_load_results_filename);
      CPPUNIT_TEST(test_load_results);
      CPPUNIT_TEST(test_conflicting_options);
      CPPUNIT_TEST(test_constructor);
    CPPUNIT_TEST_SUITE_END();
    public:
      // construction and deconstruction before tests
      void setUp();
      void tearDown();

      // test get,set and behaviour for IO_Variables 
      void test_number_of_final_results();
      void test_pattern_weighting();
      void test_print_results();
      void test_display_results();
      void test_verbose_print();
      void test_verbose_display();
      void test_save_print_to_file();
      void test_save_display_to_file();
      void test_load_puzzle_from_file();
      void test_load_config_from_file();
      void test_load_results_from_file();
      void test_print_output_filename();
      void test_display_output_filename();
      void test_puzzle_image_filename();
      void test_config_filename();
      void test_load_results_filename();
      void test_load_results();

      // test behaviour of conflicting load_results and load_puzzle
      void test_conflicting_options();
      
      // test construction of 
      void test_constructor();
    private:
      IO_Variables *tmp_var; // IO_Variables class that gets rebuilt for each test
      Record_Output record_output;  // allows test to silence & record stdout and stderr
  };

#endif
