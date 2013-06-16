#include "ww_ut_error_message.h"
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace std;
using namespace CppUnit;

CPPUNIT_TEST_SUITE_REGISTRATION(error_message_test);

void error_message_test::setUp(){
  monitor.silence();
}

void error_message_test::tearDown(){
  monitor.unsilence();
}

void error_message_test::test_number_of_final_results() {
  stringstream err;
  err << "Error: attempted to set 'number_of_final_results' to ";
  err << "-1, which should be greater than or equal to zero. ";
  err << "'number_of_final_results' will remain at 1.";
  err << endl;
  CPPUNIT_ASSERT( err_msg("number_of_final_results", -1, 1) == err.str() );
}

void error_message_test::test_pattern_weighting() {
  stringstream err;
  err << "Error: attempted to set 'pattern_weighting[\"Red and Black\"]' to ";
  err << "-1, which should be greater than zero. ";
  err << "'pattern_weighting[\"Red and Black\"]' will remain at 0.";
  err << endl;
  CPPUNIT_ASSERT( err_msg("pattern_weighting", "Red and Black", -1, 0) == err.str() );
}

void error_message_test::test_config_input_filename() {
  stringstream err;
  err << "Error: config input file 'no_such_file' does not exist.";
  err << " Continuing with 'previous_file' as config input file.";
  err << endl;
  CPPUNIT_ASSERT( err_msg("config_input_filename", "no_such_file", "previous_file") == err.str() );
  err.str("");
  err << "Error: config input file 'no_such_file' does not exist.";
  err << " Continuing with default values.";
  err << endl;
  CPPUNIT_ASSERT( err_msg("config_input_filename", "no_such_file", "") == err.str() );
}

void error_message_test::test_puzzle_input_filename() {
  stringstream err;
  err << "Error: puzzle input file 'no_such_file' does not exist.";
  err << " Continuing with 'previous_file' as puzzle input file.";
  err << endl;
  CPPUNIT_ASSERT( err_msg("puzzle_input_filename", "no_such_file", "previous_file") == err.str() );
  err.str("");
  err << "Error: puzzle input file 'no_such_file' does not exist.";
  err << " Currently, no puzzle will be solved.";
  err << endl;
  CPPUNIT_ASSERT( err_msg("puzzle_input_filename", "no_such_file", "") == err.str() );
}

void error_message_test::test_results_input_filename() {
  stringstream err;
  err << "Error: results input file 'no_such_file' does not exist.";
  err << " Continuing with 'previous_file' as results input file.";
  err << endl;
  CPPUNIT_ASSERT( err_msg("results_input_filename", "no_such_file", "previous_file") == err.str() );
  err.str("");
  err << "Error: results input file 'no_such_file' does not exist.";
  err << " Currently, no previous results will be loaded.";
  err << endl;
  CPPUNIT_ASSERT( err_msg("results_input_filename", "no_such_file", "") == err.str() );
}

void error_message_test::test_text_output_filename() {
  stringstream err;
  err << "Error: text output file 'existing_file' already exists.";
  err << " Saving text output as 'new_output_file'.";
  err << endl;
  CPPUNIT_ASSERT( err_msg("text_output_filename", "existing_file", "new_output_file") == err.str() );
}

void error_message_test::test_graphic_output_filename() {
  stringstream err;
  err << "Error: graphic output file 'existing_file' already exists.";
  err << " Saving graphic output as 'new_output_file'.";
  err << endl;
  CPPUNIT_ASSERT( err_msg("graphic_output_filename", "existing_file", "new_output_file") == err.str() );
}

void error_message_test::test_results_output_filename() {
  stringstream err;
  err << "Error: results output file 'existing_file' already exists.";
  err << " Saving results output as 'new_output_file'.";
  err << endl;
  CPPUNIT_ASSERT( err_msg("results_output_filename", "existing_file", "new_output_file") == err.str() );
}

void error_message_test::test_false_error() {
  // fake error should return error
  stringstream err;
  err << "Error Error: \"false_error\" is not a recognised error!";
  err << endl;
  CPPUNIT_ASSERT( err_msg("false_error") == err.str() );
}

int main(int argc, char* argv[]) {
  Test *suite = TestFactoryRegistry::getRegistry().makeTest();

  TextUi::TestRunner runner;
  runner.addTest( suite );

  runner.setOutputter( new CompilerOutputter(&runner.result(),cerr));

  bool wasSucessful = runner.run();
  return wasSucessful ? 0 : 1;
}
