#include "ww_ut_io_variables.h"
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace std;
using namespace CppUnit;

CPPUNIT_TEST_SUITE_REGISTRATION(io_variables_test);

void io_variables_test::setUp(){
  tmp_var = new IO_Variables();
  record_output.silence();
}
void io_variables_test::tearDown(){
  delete tmp_var;
  record_output.unsilence();
}

// test the construction of the 'IO_Variables' class
void io_variables_test::test_constructor() {
  // defaults should be as here for the various results
  CPPUNIT_ASSERT( 3 == tmp_var->get_number_of_final_results() );    // should be able to see top 3 results
  CPPUNIT_ASSERT( 0 == tmp_var->get_weighting("anything") );        // weighting should be empty at initialisation
  CPPUNIT_ASSERT( true == tmp_var->get_print_results() );           //should print results by default
  CPPUNIT_ASSERT( true == tmp_var->get_display_results() );         // should diplay results by default
  CPPUNIT_ASSERT( false == tmp_var->get_verbose_print() );          // should not print all results by default
  CPPUNIT_ASSERT( false == tmp_var->get_verbose_display() );        // should not display all results by default
  CPPUNIT_ASSERT( false == tmp_var->get_save_print_to_file() );     // should not save print output to file by default
  CPPUNIT_ASSERT( false == tmp_var->get_save_display_to_file() );   // should not save display output to file by default
  CPPUNIT_ASSERT( true == tmp_var->get_load_puzzle_from_file() );   // should try to load puzzle from file by default
  CPPUNIT_ASSERT( false == tmp_var->get_load_config_from_file() );  // should not try to load configuration from file by default
  CPPUNIT_ASSERT( false == tmp_var->get_load_results_from_file() ); // should not try to load results from file by default
  CPPUNIT_ASSERT( "" == tmp_var->get_print_output_filename() );     //*
  CPPUNIT_ASSERT( "" == tmp_var->get_display_output_filename() );   //*
  CPPUNIT_ASSERT( "" == tmp_var->get_puzzle_image_filename() );     //*** all filenames should be blank at initialisation
  CPPUNIT_ASSERT( "" == tmp_var->get_config_filename() );           //*
  CPPUNIT_ASSERT( "" == tmp_var->get_load_results_filename() );     //*
}

void io_variables_test::test_number_of_final_results() {
  // should be able to change the value of 'number_of_final_results'
  tmp_var->set_number_of_final_results(10);
  CPPUNIT_ASSERT( 10 == tmp_var->get_number_of_final_results() );

  // should ignore attempts to set number_of_final_results to values less than or equal to zero
  tmp_var->set_number_of_final_results(0);
  CPPUNIT_ASSERT( 10 == tmp_var->get_number_of_final_results() );
  tmp_var->set_number_of_final_results(-1);
  CPPUNIT_ASSERT( 10 == tmp_var->get_number_of_final_results() );

  // should print an error when bad values are included
  record_output.start_recording_cerr();
    tmp_var->set_number_of_final_results(-1);
  record_output.stop_recording_cerr();
  CPPUNIT_ASSERT( record_output.get_cerr_output() == "Error: attempted to set 'number_of_final_results' to a value less than or equal to 0. Continuing with previous value\n" );
}

void io_variables_test::test_pattern_weighting() {
  // should be able to add elements to 'pattern_weighting'
  tmp_var->set_weighting("test", 1);
  CPPUNIT_ASSERT( 1 == tmp_var->get_weighting("test") );
  
  // should be able to overwrite existing elements
  tmp_var->set_weighting("test", 2);
  CPPUNIT_ASSERT( 2 == tmp_var->get_weighting("test") );

  // should be able to add new elements without changing existing ones
  tmp_var->set_weighting("test2", 3);
  CPPUNIT_ASSERT( 3 == tmp_var->get_weighting("test2") );
  CPPUNIT_ASSERT( 2 == tmp_var->get_weighting("test") );

  // should allow a weighting to be zero
  tmp_var->set_weighting("test2", 0);
  CPPUNIT_ASSERT( 0 == tmp_var->get_weighting("test2") );

  // should ignore changes that give a weighting less than 0
  tmp_var->set_weighting("test", -1);
  CPPUNIT_ASSERT( 2 == tmp_var->get_weighting("test") );

  // should print an error for weightings set to less than 0
  record_output.start_recording_cerr();
    tmp_var->set_weighting("test", -1);
  record_output.stop_recording_cerr();
  CPPUNIT_ASSERT( record_output.get_cerr_output() == "Error: attempted to set a 'pattern_weighting' to a value less than 0. Ignoring addition.\n" );
}

void io_variables_test::test_print_results() {
  // should be able to change the value of 'print_results'
  tmp_var->set_print_results(false);
  CPPUNIT_ASSERT( false == tmp_var->get_print_results() );
  tmp_var->set_print_results(true);
  CPPUNIT_ASSERT( true == tmp_var->get_print_results() );
}

void io_variables_test::test_display_results() {
  // should be able to change the value of 'display_results'
  tmp_var->set_display_results(false);
  CPPUNIT_ASSERT( false == tmp_var->get_display_results() );
  tmp_var->set_display_results(true);
  CPPUNIT_ASSERT( true == tmp_var->get_display_results() );
}

void io_variables_test::test_verbose_print() {
  // should be able to change the value of 'verbose_print'
  tmp_var->set_verbose_print(false);
  CPPUNIT_ASSERT( false == tmp_var->get_verbose_print() );
  tmp_var->set_verbose_print(true);
  CPPUNIT_ASSERT( true == tmp_var->get_verbose_print() );
}

void io_variables_test::test_verbose_display() {
  // should be able to change the value of 'verbose_display'
  tmp_var->set_verbose_display(false);
  CPPUNIT_ASSERT( false == tmp_var->get_verbose_display() );
  tmp_var->set_verbose_display(true);
  CPPUNIT_ASSERT( true == tmp_var->get_verbose_display() );
}

void io_variables_test::test_save_print_to_file() {
  // should be able to change the value of 'save_print_to_file'
  tmp_var->set_save_print_to_file(false);
  CPPUNIT_ASSERT( false == tmp_var->get_save_print_to_file() );
  tmp_var->set_save_print_to_file(true);
  CPPUNIT_ASSERT( true == tmp_var->get_save_print_to_file() );
}

void io_variables_test::test_save_display_to_file() {
  // should be able to change the value of 'save_display_to_file'
  tmp_var->set_save_display_to_file(false);
  CPPUNIT_ASSERT( false == tmp_var->get_save_display_to_file() );
  tmp_var->set_save_display_to_file(true);
  CPPUNIT_ASSERT( true == tmp_var->get_save_display_to_file() );
}

void io_variables_test::test_load_puzzle_from_file() {
  // should be able to change the value of 'load_puzzle_from_file'
  tmp_var->set_load_puzzle_from_file(false);
  CPPUNIT_ASSERT( false == tmp_var->get_load_puzzle_from_file() );
  tmp_var->set_load_puzzle_from_file(true);
  CPPUNIT_ASSERT( true == tmp_var->get_load_puzzle_from_file() );
}

void io_variables_test::test_load_config_from_file() {
  // should be able to change the value of 'load_config_from_file'
  tmp_var->set_load_config_from_file(false);
  CPPUNIT_ASSERT( false == tmp_var->get_load_config_from_file() );
  tmp_var->set_load_config_from_file(true);
  CPPUNIT_ASSERT( true == tmp_var->get_load_config_from_file() );
}

void io_variables_test::test_load_results_from_file() {
  // should be able to change the value of 'load_results_from_file'
  tmp_var->set_load_results_from_file(false);
  CPPUNIT_ASSERT( false == tmp_var->get_load_results_from_file() );
  tmp_var->set_load_results_from_file(true);
  CPPUNIT_ASSERT( true == tmp_var->get_load_results_from_file() );
}

void io_variables_test::test_print_output_filename(){
  // should be able to change the value of 'print_output_filename'
  string file = "new_file";
  tmp_var->set_print_output_filename("new_file");
  CPPUNIT_ASSERT( "new_file" == tmp_var->get_print_output_filename() );
}

void io_variables_test::test_display_output_filename(){
  // should be able to change the value of 'display_output_filename'
  tmp_var->set_display_output_filename("new_file");
  CPPUNIT_ASSERT( "new_file" == tmp_var->get_display_output_filename() );
}

void io_variables_test::test_puzzle_image_filename(){
  // should be able to change the value of 'puzzle_image_filename' to an existing file
  tmp_var->set_puzzle_image_filename("../../sample_files/test_puzzle_image.png");
  CPPUNIT_ASSERT( "../../sample_files/test_puzzle_image.png" == tmp_var->get_puzzle_image_filename() );

  // should ignore attempts to change the filename to a file that does not exist
  tmp_var->set_puzzle_image_filename("../../sample_files/no_such_file");
  CPPUNIT_ASSERT( "../../sample_files/test_puzzle_image.png" == tmp_var->get_puzzle_image_filename() );

  // should print an error with attempts to change the filename to a file that does not exist
  record_output.start_recording_cerr();
    tmp_var->set_puzzle_image_filename("../../sample_files/no_such_file");
  record_output.stop_recording_cerr();
  CPPUNIT_ASSERT( record_output.get_cerr_output() == "Error: attempted to set 'puzzle_image_filename' to a file that does not exist. Continuing with previous value.\n" );

  // should be able to set the value of 'puzzle_image_filename' to a blank one
  tmp_var->set_puzzle_image_filename("");
  CPPUNIT_ASSERT( "" == tmp_var->get_puzzle_image_filename() );
}

void io_variables_test::test_config_filename(){
  // should be able to change the value of 'config_filename' to an existing file
  tmp_var->set_config_filename("../../sample_files/test_config.cfg");
  CPPUNIT_ASSERT( "../../sample_files/test_config.cfg" == tmp_var->get_config_filename() );

  // should ignore attempts to change the filename to a file that does not exist
  tmp_var->set_config_filename("../../sample_files/no_such_file");
  CPPUNIT_ASSERT( "../../sample_files/test_config.cfg" == tmp_var->get_config_filename() );

  // should print an error with attempts to change the filename to a file that does not exist
  record_output.start_recording_cerr();
    tmp_var->set_config_filename("../../sample_files/no_such_file");
  record_output.stop_recording_cerr();
  CPPUNIT_ASSERT( record_output.get_cerr_output() == "Error: attempted to set 'config_filename' to a file that does not exist. Continuing with previous value.\n" );

  // should be able to set the value of 'config_filename' to a blank one
  tmp_var->set_config_filename("");
  CPPUNIT_ASSERT( "" == tmp_var->get_config_filename() );
}

void io_variables_test::test_load_results_filename(){
  // should be able to change the value of 'load_results_filename' to an existing file
  tmp_var->set_load_results_filename("../../sample_files/test_load_results.dat");
  CPPUNIT_ASSERT( "../../sample_files/test_load_results.dat" == tmp_var->get_load_results_filename() );

  // should ignore attempts to change the filename to a file that does not exist
  tmp_var->set_load_results_filename("../../sample_files/no_such_file");
  CPPUNIT_ASSERT( "../../sample_files/test_load_results.dat" == tmp_var->get_load_results_filename() );

  // should print an error with attempts to change the filename to a file that does not exist
  record_output.start_recording_cerr();
    tmp_var->set_load_results_filename("../../sample_files/no_such_file");
  record_output.stop_recording_cerr();
  CPPUNIT_ASSERT( record_output.get_cerr_output() == "Error: attempted to set 'load_results_filename' to a file that does not exist. Continuing with previous value.\n" );

  // should be able to set the value of 'load_results_filename' to a blank one
  tmp_var->set_load_results_filename("");
  CPPUNIT_ASSERT( "" == tmp_var->get_load_results_filename() );
}

void io_variables_test::test_load_results() {
  Pattern_Result tmp_result1;
    tmp_result1.info.set_name("red and black");
    tmp_result1.wally_location[0] = 10;
    tmp_result1.wally_location[1] = 11;
    tmp_result1.scale[0] = 20;
    tmp_result1.scale[1] = 21;
    tmp_result1.certainty = 0.5;
  Pattern_Result tmp_result2;
    tmp_result1.info.set_name("stripes");
    tmp_result1.wally_location[0] = 50;
    tmp_result1.wally_location[1] = 70;
    tmp_result1.scale[0] = 3;
    tmp_result1.scale[1] = 40;
    tmp_result1.certainty = 2.0;
  vector<Pattern_Result> tmp_result_list;
  tmp_result_list.push_back(tmp_result1);
  tmp_result_list.push_back(tmp_result2);
   
  // should be able to add pattern_results to 'load_results'
  tmp_var->add_to_load_results(tmp_result_list);
  CPPUNIT_ASSERT(tmp_result_list == tmp_var->get_load_results());
}

void io_variables_test::test_conflicting_options() {
  // load_results_from_file and load_puzzle_from_file should never have the same value (need input from one of them)
  tmp_var->set_load_puzzle_from_file(true);
  tmp_var->set_load_results_from_file(true);
  CPPUNIT_ASSERT( tmp_var->get_load_results_from_file() != tmp_var->get_load_puzzle_from_file() );
  tmp_var->set_load_puzzle_from_file(false);
  tmp_var->set_load_results_from_file(false);
  CPPUNIT_ASSERT( tmp_var->get_load_results_from_file() != tmp_var->get_load_puzzle_from_file() );
 
  // most recently set value should take precedence
  tmp_var->set_load_results_from_file(true);
  tmp_var->set_load_puzzle_from_file(true);
  CPPUNIT_ASSERT( tmp_var->get_load_results_from_file() == false );
  CPPUNIT_ASSERT( tmp_var->get_load_puzzle_from_file() == true );

  tmp_var->set_load_results_from_file(false);
  tmp_var->set_load_puzzle_from_file(false);
  CPPUNIT_ASSERT( tmp_var->get_load_results_from_file() == true );
  CPPUNIT_ASSERT( tmp_var->get_load_puzzle_from_file() == false );

  // should print an error when a change is forced
  record_output.start_recording_cerr();
    tmp_var->set_load_puzzle_from_file(true);
  record_output.stop_recording_cerr();
  CPPUNIT_ASSERT( record_output.get_cerr_output() == "\t Setting 'load_results_from_file' to false and 'load_puzzle_from_file' to true.\n" );
  record_output.start_recording_cerr();
    tmp_var->set_load_puzzle_from_file(false);
  record_output.stop_recording_cerr();
  CPPUNIT_ASSERT( record_output.get_cerr_output() == "\t Setting 'load_results_from_file' to true and 'load_puzzle_from_file' to false.\n" );
}

int main(int argc, char* argv[]) {
  Test *suite = TestFactoryRegistry::getRegistry().makeTest();

  TextUi::TestRunner runner;
  runner.addTest( suite );

  runner.setOutputter( new CompilerOutputter(&runner.result(),cerr));

  bool wasSucessful = runner.run();
  return wasSucessful ? 0 : 1;
}
