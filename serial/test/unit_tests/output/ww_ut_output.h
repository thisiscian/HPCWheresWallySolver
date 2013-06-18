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
      CPPUNIT_TEST(test_final_results);
      CPPUNIT_TEST(test_show_text_results);
      CPPUNIT_TEST(test_save_text_results);
      CPPUNIT_TEST(test_show_graphic_results);
      CPPUNIT_TEST(test_save_graphic_results);
      CPPUNIT_TEST(test_save_results_to_file);
    CPPUNIT_TEST_SUITE_END();
    public:
      void setUp();
      void tearDown();

      void test_final_results();
      void test_show_text_results();
      void test_save_text_results();
      void test_show_graphic_results();
      void test_save_graphic_results();
      void test_save_results_to_file();

    private:
      Output *output;
      IO_Variables *var;
      std::vector<Pattern_Result> final_results;
      IO_Monitor monitor;
  };

#endif
