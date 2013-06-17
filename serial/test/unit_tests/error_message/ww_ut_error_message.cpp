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

void error_message_test::test_option_config(){
  stringstream err;
  err << "Error: " << "--config=" << " expects a single filename as an argument\n";
  err << "\te.g. " << "--config=" << "sample_files/test_config.cfg";
  err << endl;
  CPPUNIT_ASSERT( err_msg("option_config", "--config=") == err.str() );
}

void error_message_test::test_option_number_of_results() {
  stringstream err;
  err << "Error: " << "--number-of-final-results=" << " expects a single integer as an argument\n";
  err << "\te.g. " << "--number-of-final-results=" << "10";
  err << endl;
  CPPUNIT_ASSERT( err_msg("option_number_of_final_results", "--number-of-final-results=") == err.str() );
}

void error_message_test::test_option_pattern_weighting() {
  stringstream err;
  err << "Error: " << "--pattern-weighting=" << " expects a comma separated list of patterns and weightings\n";
  err << "\te.g. " << "--pattern-weighting=" << "\"Red and Black\":2,\"Stripes\":1,\"Wally\":5";
  err << endl;
  CPPUNIT_ASSERT( err_msg("option_pattern_weighting", "--pattern-weighting=") == err.str() );
}

void error_message_test::test_option_load_puzzle() {
  stringstream err;
  err << "Error: " << "--load-puzzle=" << " expects a single filename as an argument\n";
  err << "\te.g. " << "--load-puzzle=" << "sample_files/test_wally_puzzle.png";
  err << endl;
  CPPUNIT_ASSERT( err_msg("option_load_puzzle", "--load-puzzle=") == err.str() );
}

void error_message_test::test_option_load_results() {
  stringstream err;
  err << "Error: " << "--load-results=" << " expects a single filename as an argument\n";
  err << "\te.g. " << "--load-results=" << "sample_files/test_results.dat";
  err << endl;
  CPPUNIT_ASSERT( err_msg("option_load_results", "--load-results=") == err.str() );
}

void error_message_test::test_option_show_text() {
  stringstream err;
  err << "Error: " << "--show-text=" << " expects TRUE, FALSE or VERBOSE as an argument \n";
  err << "\te.g. " << "--show-text=" << "TRUE";
  err << endl;
  CPPUNIT_ASSERT( err_msg("option_show_text", "--show-text=") == err.str() );
}

void error_message_test::test_option_show_graphic() {
  stringstream err;
  err << "Error: " << "--show-graphic=" << " expects TRUE, FALSE or VERBOSE as an argument \n";
  err << "\te.g. " << "--show-graphic=" << "TRUE";
  err << endl;
  CPPUNIT_ASSERT( err_msg("option_show_graphic", "--show-graphic=") == err.str() );
}

void error_message_test::test_option_save_text() {
  stringstream err;
  err << "Error: " << "--save-text=" << " expects a single filename as an argument\n";
  err << "\te.g. " << "--save-text=" << "sample_files/test_text_output.txt";
  err << endl;
  CPPUNIT_ASSERT( err_msg("option_save_text", "--save-text=") == err.str() );
}

void error_message_test::test_option_save_graphic() {
  stringstream err;
  err << "Error: " << "--save-graphic=" << " expects a single filename as an argument\n";
  err << "\te.g. " << "--save-graphic=" << "sample_files/test_graphic_output.png";
  err << endl;
  CPPUNIT_ASSERT( err_msg("option_save_graphic", "--save-graphic=") == err.str() );
}

void error_message_test::test_option_save_results() {
  stringstream err;
  err << "Error: " << "--save-results=" << " expects a single filename as an argument\n";
  err << "\te.g. " << "--save-results=" << "sample_files/test_results.dat";
  err << endl;
  CPPUNIT_ASSERT( err_msg("option_save_results", "--save-results=") == err.str() );
}


void error_message_test::test_option_help() {
  stringstream err;
  err << "usage: " << "./test_app" << " [options [files]]" << endl;
  err << "\t-c,--config=CONFIG_FILE\tSets the puzzle solving configuration as described by the given file" << endl;
  err << "\t-n,--number-of-final-results=N\tSets the number of results to be output" << endl;
  err << "\t-w,--pattern-weighting=\"NAME\":WEIGHT[,\"NAME\":WEIGHT[,...]]]\tSets the weighting for each pattern" << endl;
  err << "\t-p,--load-puzzle=FILE\tAttempts to locate Wally in FILE" << endl;
  err << "\t-l,--load-results=RESULTS_FILE\tLoads results from a previous execution" << endl;
  err << "\t-Op,--show-text={TRUE,FALSE,VERBOSE}\tPrints the results to the terminal" << endl;
  err << "\t-Od,--show-graphic={TRUE,FALSE,VERBOSE}\tShows the results graphically, in a new window" << endl;
  err << "\t-St,--save-text=FILE\tSaves the textual format of the results to FILE" << endl;
  err << "\t-Si,--save-graphic=FILE\tSaves the graphical format of the results to FILE" << endl;
  err << "\t-Sr,--save-results=FILE\tSaves the results to FILE, which can be loaded later" << endl;
  err << "\t-h,--help\tDisplays this help message" << endl;

  CPPUNIT_ASSERT( err_msg("option_help", "./test_app") == err.str() );
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
