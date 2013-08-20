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
  err << "-1, which should be greater than zero. ";
  err << "'number_of_final_results' will remain at 1.";
  err << endl;
  CPPUNIT_ASSERT( err_msg("number_of_final_results", -1, 1) == err.str() );
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

void error_message_test::test_option_number_of_results() {
  stringstream err;
  err << "Error: " << "--number-of-final-results=" << " expects a single integer as an argument\n";
  err << "\te.g. " << "--number-of-final-results=" << "10";
  err << endl;
  CPPUNIT_ASSERT( err_msg("option_number_of_final_results", "--number-of-final-results=") == err.str() );
}

void error_message_test::test_option_load_puzzle() {
  stringstream err;
  err << "Error: " << "--load-puzzle=" << " expects a single filename as an argument\n";
  err << "\te.g. " << "--load-puzzle=" << "example_input.png";
  err << endl;
  CPPUNIT_ASSERT( err_msg("option_load_puzzle", "--load-puzzle=") == err.str() );
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

void error_message_test::test_option_help() {
  stringstream err;
  err << "usage: " << "./test_app" << " [flag [option]] [file]" << endl;
  err << "\t-omp,--openmp=N\tSets the number of threads that OpenMP can use" << endl;
  err << "\t-n,--number-of-final-results=N\tSets the number of results to be output" << endl;
  err << "\t-p,--load-puzzle=FILE\tAttempts to locate Wally in FILE" << endl;
  err << "\t-Oc,--show-text={TRUE,FALSE,VERBOSE}\tPrints the results to the terminal" << endl;
  err << "\t-Og,--show-graphic={TRUE,FALSE,VERBOSE}\tShows the results graphically, in a new window" << endl;
  err << "\t-Ot,--show-timing={TRUE,FALSE}\tPrints the timing results to the terminal" << endl;
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
