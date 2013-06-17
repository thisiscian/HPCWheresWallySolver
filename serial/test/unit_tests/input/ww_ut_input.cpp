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

void input_test::test_option_config() {
  // should reject badly parsed options
  int argc = 3;
  char *argv[argc];
    argv[0] = new char[255];
    strcpy(argv[0], "./test_program_name");
    argv[1] = new char[255];
    strcpy(argv[1], "-c");
    argv[2] = new char[255];
    strcpy(argv[1], "-p");
  monitor.start_recording_cerr();
  input->parse_input(argc, argv);
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT( false == input->variables->get_load_config_from_file() );
  CPPUNIT_ASSERT( "" == input->variables->get_config_input_filename() );

  delete argv[0];
  delete argv[1];
  delete argv[2];
  // should be able to handle '--config=...' style input
  argc = 2;
   *argv[argc];
    argv[0] = new char[255];
    strcpy(argv[0], "./test_program_name");
    argv[1] = new char[255];
    strcpy(argv[1], "--config=../../sample_files/test_config.cfg");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( true == input->variables->get_load_config_from_file() );
  CPPUNIT_ASSERT( "../../sample_files/test_config.cfg" == input->variables->get_config_input_filename() );
  

  input->variables->set_load_config_from_file(false);
  input->variables->set_config_input_filename("");

  // should be able to handle '-c' style input
  argc = 3;
  delete argv[1];
    argv[1] = new char[255];
    strcpy(argv[1], "-c");
    argv[2] = new char[255];
    strcpy(argv[2], "../../sample_files/test_config.cfg");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( true == input->variables->get_load_config_from_file() );
  CPPUNIT_ASSERT( "../../sample_files/test_config.cfg" == input->variables->get_config_input_filename() );
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
void input_test::test_option_pattern_weighting() {
  // should be able to handle '--pattern-weighting=...' style input
  int argc = 2;
  char *argv[argc];
    argv[0] = new char[255];
    strcpy(argv[0], "./test_program_name");
    argv[1] = new char[255];
    strcpy(argv[1], "--pattern-weighting=\"Red and Black\":1,\"Stripes\":2");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( 1 == input->variables->get_weighting("Red and Black") );
  CPPUNIT_ASSERT( 2 == input->variables->get_weighting("Stripes") );

  // should be able to handle '-w' style input
  argc = 3;
  delete argv[1];
    argv[1] = new char[255];
    strcpy(argv[1], "-w");
    argv[2] = new char[255];
    strcpy(argv[2], "\"RedBlack\":1,\"Stres\":2");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( 1 == input->variables->get_weighting("RedBlack") );
  CPPUNIT_ASSERT( 2 == input->variables->get_weighting("Stres") );
}

void input_test::test_option_solve_puzzle() {
  // should be able to handle '--solve-puzzle=...' style input
  int argc = 2;
  char *argv[argc];
    argv[0] = new char[255];
    strcpy(argv[0], "./test_program_name");
    argv[1] = new char[255];
    strcpy(argv[1], "--solve-puzzle=../../sample_files/test_puzzle_input.png");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( true == input->variables->get_load_puzzle_from_file() );
  CPPUNIT_ASSERT( "../../sample_files/test_puzzle_input.png" == input->variables->get_puzzle_input_filename() );

  input->variables->set_load_puzzle_from_file(false);
  input->variables->set_puzzle_input_filename("");

  // should be able to handle '-p' style input
  argc = 3;
  delete argv[1];
    argv[1] = new char[255];
    strcpy(argv[1], "-p");
    argv[2] = new char[255];
  strcpy(argv[2], "../../sample_files/test_puzzle_input.png");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( true == input->variables->get_load_puzzle_from_file() );
  CPPUNIT_ASSERT( "../../sample_files/test_puzzle_input.png" == input->variables->get_puzzle_input_filename() );
 
}

void input_test::test_option_load_results() {
  // should be able to handle '--load-results=...' style input
  int argc = 2;
  char *argv[argc];
    argv[0] = new char[255];
    strcpy(argv[0], "./test_program_name");
    argv[1] = new char[255];
    strcpy(argv[1], "--load-results=../../sample_files/test_load_results.dat");
  input->parse_input(argc, argv);
  if( input->variables->get_results_input_filename() == "../../sample_files/test_load_results.dat") cerr << "OKAY" << endl;
  CPPUNIT_ASSERT( true == input->variables->get_load_results_from_file() );
  CPPUNIT_ASSERT( "../../sample_files/test_load_results.dat" == input->variables->get_results_input_filename() );

  input->variables->set_load_results_from_file(false);
  input->variables->set_puzzle_input_filename("");

  // should be able to handle '-l' style input
  argc = 3;
  delete argv[1];
    argv[1] = new char[255];
    strcpy(argv[1], "-l");
    argv[2] = new char[255];
    strcpy(argv[2], "../../sample_files/test_load_results.dat");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( true == input->variables->get_load_results_from_file() );
  CPPUNIT_ASSERT( "../../sample_files/test_load_results.dat" == input->variables->get_results_input_filename() );
}

void input_test::test_option_text_output() {
  // should be able to handle '--print-output=...' style input
  int argc = 2;
  char *argv[argc];
    argv[0] = new char[255];
    strcpy(argv[0], "./test_program_name");
    argv[1] = new char[255];
    strcpy(argv[1], "--print-output=FALSE");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( false == input->variables->get_show_text_results() );

  // should be able to handle '-Op' style input
  argc = 3;
  delete argv[1];
    argv[1] = new char[255];
    strcpy(argv[1], "-Op");
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
    strcpy(argv[1], "--display-output=FALSE");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( false == input->variables->get_show_graphic_results() );

  // should be able to handle '-Od' style input
  argc = 3;
  delete argv[1];
    argv[1] = new char[255];
    strcpy(argv[1], "-Od");
    argv[2] = new char[255];
    strcpy(argv[2], "VERBOSE");
    input->parse_input(argc, argv);
  CPPUNIT_ASSERT( true == input->variables->get_make_graphic_verbose() );
  CPPUNIT_ASSERT( true == input->variables->get_show_graphic_results() );
}

void input_test::test_option_save_text_output() {
  // should be able to handle '--save-text-output=...' style input
  int argc = 2;
  char *argv[argc];
    argv[0] = new char[255];
    strcpy(argv[0], "./test_program_name");
    argv[1] = new char[255];
    strcpy(argv[1], "--save-text-output=../../sample_files/test_text_output.dat");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( true == input->variables->get_save_text_to_file() );
  CPPUNIT_ASSERT( "../../sample_files/test_text_output.dat" == input->variables->get_text_output_filename() );

  input->variables->set_load_results_from_file(false);
  input->variables->set_puzzle_input_filename("");

  // should be able to handle '-St' style input
  argc = 3;
  delete argv[1];
    argv[1] = new char[255];
    strcpy(argv[1], "-St");
    argv[2] = new char[255];
    strcpy(argv[2], "../../sample_files/test_text_output.dat");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( true == input->variables->get_save_text_to_file() );
  CPPUNIT_ASSERT( "../../sample_files/test_text_output.dat" == input->variables->get_text_output_filename() );
}

void input_test::test_option_save_graphic_output() {
  // should be able to handle '--save-print-output=...' style input
  int argc = 2;
  char *argv[argc];
    argv[0] = new char[255];
    strcpy(argv[0], "./test_program_name");
    argv[1] = new char[255];
    strcpy(argv[1], "--save-image-output=../../sample_files/test_image_output.dat");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( true == input->variables->get_save_graphic_to_file() );
  CPPUNIT_ASSERT( "../../sample_files/test_image_output.dat" == input->variables->get_graphic_output_filename() );

  input->variables->set_load_results_from_file(false);
  input->variables->set_puzzle_input_filename("");

  // should be able to handle '-Si' style input
  argc = 3;
  delete argv[1];
    argv[1] = new char[255];
    strcpy(argv[1], "-Si");
    argv[2] = new char[255];
    strcpy(argv[2], "../../sample_files/test_image_output.dat");
  input->parse_input(argc, argv);
  CPPUNIT_ASSERT( true == input->variables->get_save_graphic_to_file() );
  CPPUNIT_ASSERT( "../../sample_files/test_image_output.dat" == input->variables->get_graphic_output_filename() );
}

void input_test::test_option_help() {
  // should print normal help with no extra arguments
  int argc = 2;
  char *argv[argc];
    argv[0] = new char[255];
    strcpy(argv[0], "./test_program_name");
    argv[1] = new char[255];
    strcpy(argv[1], "--help");
  stringstream help_message;

  help_message << "usage: " << argv[0] << " [options [files]]" << endl;
  help_message << "\t-c,--config=CONFIG_FILE\tSets the puzzle solving configuration as described by the given file" << endl;
  help_message << "\t-n,--number-of-final-results=N\tSets the number of results to be output" << endl;
  help_message << "\t-w,--pattern-weighting=\"NAME\":WEIGHT[,\"NAME\":WEIGHT[,...]]]\tSets the weighting for each pattern" << endl;
  help_message << "\t-p,--solve-puzzle=PUZZLE_FILE\tAttempts to locate Wally in the PUZZLE_FILE" << endl;
  help_message << "\t-l,--load-results=RESULTS_FILE\tLoads results from a previous execution" << endl;
  help_message << "\t-Op,--print-output={TRUE,FALSE,VERBOSE}\tPrints the results to the terminal" << endl;
  help_message << "\t-Od,--display-output={TRUE,FALSE,VERBOSE}\tShows the results graphically, in a new window" << endl;
  help_message << "\t-St,--save-text-output=OUTPUT_FILE\tSaves the textual format of the results to OUTPUT_FILE" << endl;
  help_message << "\t-Si,--save-image-output=OUTPUT_FILE\tSaves the graphical format of the results to OUTPUT_FILE" << endl;
  help_message << "\t-h,--help\tDisplays this help message" << endl;

  monitor.start_recording_cerr();
    input->parse_input(argc,argv);
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT(monitor.get_cerr_output() == help_message.str());
  delete argv[1];
    argv[1] = new char[255];
    strcpy(argv[1], "-h");

  monitor.start_recording_cerr();
    input->parse_input(argc,argv);
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT(monitor.get_cerr_output() == help_message.str());
}

void input_test::test_load_config() {
  // should not load 'bad' config files, should return error
  input->variables->set_config_input_filename("../../sample_files/test_config_bad.cfg");
  monitor.start_recording_cerr();
    int err_no = input->load_config();
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT( BAD_CONFIG_FILE == err_no );
  //CPPUNIT_ASSERT( monitor.get_cerr_output() == "Error: config file contained some errors\n" );
  

  // should be able to load a 'good' config file
  input->variables->set_config_input_filename("../../sample_files/test_config.cfg");
  CPPUNIT_ASSERT( 0 == input->load_config() );

  // loading a config file should change the values of the io_variable linked to the input class
  CPPUNIT_ASSERT( 4 == input->variables->get_number_of_final_results() );
  CPPUNIT_ASSERT( 1 == input->variables->get_weighting("red and black") );
  CPPUNIT_ASSERT( 4 == input->variables->get_weighting("stripes") );
  CPPUNIT_ASSERT( 2 == input->variables->get_weighting("wally") );
  CPPUNIT_ASSERT( false == input->variables->get_show_text_results() );
  CPPUNIT_ASSERT( false == input->variables->get_show_graphic_results() );
  CPPUNIT_ASSERT( true == input->variables->get_make_graphic_verbose() );
  CPPUNIT_ASSERT( true == input->variables->get_make_text_verbose() );
  CPPUNIT_ASSERT( true == input->variables->get_save_text_to_file() );
  CPPUNIT_ASSERT( true == input->variables->get_save_graphic_to_file() );
  CPPUNIT_ASSERT( false == input->variables->get_load_puzzle_from_file() );
  CPPUNIT_ASSERT( true == input->variables->get_load_results_from_file() );
  CPPUNIT_ASSERT( "../../sample_files/test_save_graphic.png" == input->variables->get_graphic_output_filename() );
  CPPUNIT_ASSERT( "../../sample_files/test_save_text.dat" == input->variables->get_text_output_filename() );
  CPPUNIT_ASSERT( "../../sample_files/test_puzzle_input.png" == input->variables->get_puzzle_input_filename() );
  CPPUNIT_ASSERT( "../../sample_files/test_load_results.dat" == input->variables->get_results_input_filename() );
}

void input_test::test_load_results() {
  // should not load a 'bad' results file, should return error
  input->variables->set_results_input_filename("../../sample_files/test_load_results_bad.dat");
  monitor.start_recording_cerr();
    int err_no = input->load_results();
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT( BAD_RESULTS_FILE == err_no );
  CPPUNIT_ASSERT( monitor.get_cerr_output() == "Error: results file contained some errors\n" );

  // should be able to load a 'good' results file
  input->variables->set_results_input_filename("../../sample_files/test_load_results.dat");
  CPPUNIT_ASSERT( 0 == input->load_results() );

  // should store loaded results in 'variables->load_results'
  vector<Pattern_Result> expected_results;
    Pattern_Result result1;
      result1.info.set_name("black and red"); 
      result1.info.set_description("very good");
      result1.info.set_confidence(1.0);
      result1.wally_location[0] = 10; 
      result1.wally_location[1] = 12; 
      result1.scale[0] = 10;
      result1.scale[1] = 10;
      result1.certainty = 1.0;
    Pattern_Result result2;
      result2.info.set_name("stripes"); 
      result2.info.set_description("quite bad");
      result2.info.set_confidence(0.1);
      result2.wally_location[0] = 200; 
      result2.wally_location[1] = 200; 
      result2.scale[0] = 300;
      result2.scale[1] = 300;
      result2.certainty = 0.1;
  expected_results.push_back(result1);
  expected_results.push_back(result2);
  CPPUNIT_ASSERT( expected_results == input->variables->get_loaded_results() );
}

void input_test::test_unrecognised_command() {
  int argc = 2;
  char *argv[argc];
    argv[0] = new char[255];
    strcpy(argv[0], "./test_program_name");
    argv[1] = new char[255];
    strcpy(argv[1], "this_is_not_a_command");
  monitor.start_recording_cerr();
    int err_no = input->parse_input(argc, argv);
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT( UNRECOGNISED_COMMAND == err_no);
  CPPUNIT_ASSERT( "Error: unrecognised command, exiting\n" == monitor.get_cerr_output());
}

int main(int argc, char* argv[]) {
  Test *suite = TestFactoryRegistry::getRegistry().makeTest();

  TextUi::TestRunner runner;
  runner.addTest( suite );

  runner.setOutputter( new CompilerOutputter(&runner.result(),cerr));

  bool wasSucessful = runner.run();
  return wasSucessful ? 0 : 1;
}
