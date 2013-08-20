#include "ww_ut_input.h"
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace std;
using namespace CppUnit;

CPPUNIT_TEST_SUITE_REGISTRATION(input_test);

void input_test::setUp(){
  var = new IO_Variables();
  input = new Input(var);
  monitor.silence();
}

void input_test::tearDown(){
  monitor.unsilence();
  delete input;
  delete var;
}

// test the construction of the 'Input' class
void input_test::test_constructor() {
 CPPUNIT_ASSERT(var == input->variables);  // class variable  "variables" should be changed to input variable
}

void input_test::test_no_arguments() {
  int argc = 1;
  char *argv[argc];
    argv[0] = new char[255];
    strcpy(argv[0], "./test_program_name");

  // should print help with no arguments
  monitor.start_recording_cerr();
    input->parse_input(argc,argv);
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT(monitor.get_cerr_output() == err_msg("option_help", argv[0]));
}

void input_test::test_option_number_of_final_results() {
  // should be able to handle '--number-of-final-results=...' style input
  int argc = 2;
  char *argv[argc];
    argv[0] = new char[255];
    strcpy(argv[0], "./test_program_name");
    argv[1] = new char[255];
    strcpy(argv[1], "--number-of-final-results=5");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( 5 == input->variables->get_number_of_final_results() );

  input->variables->set_number_of_final_results(2);

  // should be able to handle '-n' style input
  argc = 3;
  delete argv[1];
    argv[1] = new char[255];
    strcpy(argv[1], "-n");
    argv[2] = new char[255];
    strcpy(argv[2], "4");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( 4 == input->variables->get_number_of_final_results() );
 
}

void input_test::test_option_solve_puzzle() {
  // should be able to handle '--solve-puzzle=...' style input
  int argc = 2;
  char *argv[argc];
    argv[0] = new char[255];
    strcpy(argv[0], "./test_program_name");
    argv[1] = new char[255];
    strcpy(argv[1], "--load-puzzle=../../sample_files/test_puzzle_image.png");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( true == input->variables->get_load_puzzle_from_file() );
  CPPUNIT_ASSERT( "../../sample_files/test_puzzle_image.png" == input->variables->get_puzzle_input_filename() );

  input->variables->set_load_puzzle_from_file(false);
  input->variables->set_puzzle_input_filename("");

  // should be able to handle '-p' style input
  argc = 3;
  delete argv[1];
    argv[1] = new char[255];
    strcpy(argv[1], "-p");
    argv[2] = new char[255];
  strcpy(argv[2], "../../sample_files/test_puzzle_image.png");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( true == input->variables->get_load_puzzle_from_file() );
  CPPUNIT_ASSERT( "../../sample_files/test_puzzle_image.png" == input->variables->get_puzzle_input_filename() );
 
}

void input_test::test_option_text_output() {
  // should be able to handle '--print-output=...' style input
  int argc = 2;
  char *argv[argc];
    argv[0] = new char[255];
    strcpy(argv[0], "./test_program_name");
    argv[1] = new char[255];
    strcpy(argv[1], "--show-text=FALSE");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( false == input->variables->get_show_text_results() );

  // should be able to handle '-Op' style input
  argc = 3;
  delete argv[1];
    argv[1] = new char[255];
    strcpy(argv[1], "-Oc");
    argv[2] = new char[255];
    strcpy(argv[2], "VERBOSE");
    input->parse_input(argc, argv);
  CPPUNIT_ASSERT( true == input->variables->get_make_text_verbose() );
  CPPUNIT_ASSERT( true == input->variables->get_show_text_results() );
}


void input_test::test_option_graphic_output() {
  // should be able to handle '--display-output=...' style input
  int argc = 2;
  char *argv[argc];
    argv[0] = new char[255];
    strcpy(argv[0], "./test_program_name");
    argv[1] = new char[255];
    strcpy(argv[1], "--show-graphic=FALSE");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( false == input->variables->get_show_graphic_results() );

  // should be able to handle '-Od' style input
  argc = 3;
  delete argv[1];
    argv[1] = new char[255];
    strcpy(argv[1], "-Og");
    argv[2] = new char[255];
    strcpy(argv[2], "VERBOSE");
    input->parse_input(argc, argv);
  CPPUNIT_ASSERT( true == input->variables->get_make_graphic_verbose() );
  CPPUNIT_ASSERT( true == input->variables->get_show_graphic_results() );
}

void input_test::test_option_help() {
  // should print normal help with no extra arguments
  int argc = 2;
  char *argv[argc];
    argv[0] = new char[255];
    strcpy(argv[0], "./test_program_name");
    argv[1] = new char[255];
    strcpy(argv[1], "--help");

  monitor.start_recording_cerr();
    input->parse_input(argc,argv);
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT(monitor.get_cerr_output() == err_msg("option_help", argv[0]));
  delete argv[1];
    argv[1] = new char[255];
    strcpy(argv[1], "-h");

  monitor.start_recording_cerr();
    input->parse_input(argc,argv);
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT(monitor.get_cerr_output() == err_msg("option_help", argv[0]));
}

int main(int argc, char* argv[]) {
  Test *suite = TestFactoryRegistry::getRegistry().makeTest();

  TextUi::TestRunner runner;
  runner.addTest( suite );

  runner.setOutputter( new CompilerOutputter(&runner.result(),cerr));

  bool wasSucessful = runner.run();
  return wasSucessful ? 0 : 1;
}
