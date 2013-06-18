#include "ww_ut_io_variables.h"

using namespace std;
using namespace CppUnit;
using namespace cv;

CPPUNIT_TEST_SUITE_REGISTRATION(io_variables_test);

void io_variables_test::setUp(){
  tmp_var = new IO_Variables();
  monitor.silence();
}
void io_variables_test::tearDown(){
  delete tmp_var;
  monitor.unsilence();
}

void io_variables_test::test_load_config_from_file() {
  // should be able to change the value of 'load_config_from_file'
  tmp_var->set_load_config_from_file(false);
  CPPUNIT_ASSERT( false == tmp_var->get_load_config_from_file() );
  tmp_var->set_load_config_from_file(true);
  CPPUNIT_ASSERT( true == tmp_var->get_load_config_from_file() );
}

void io_variables_test::test_config_input_filename(){
  // should be able to change the value of 'config_input_filename' to an existing file
  tmp_var->set_config_input_filename("../../sample_files/test_config.cfg");
  CPPUNIT_ASSERT( "../../sample_files/test_config.cfg" == tmp_var->get_config_input_filename() );

  // should ignore attempts to change the filename to a file that does not exist
  tmp_var->set_config_input_filename("../../sample_files/no_such_file");
  CPPUNIT_ASSERT( "../../sample_files/test_config.cfg" == tmp_var->get_config_input_filename() );

  // should print an error with attempts to change the filename to a file that does not exist
  monitor.start_recording_cerr();
    tmp_var->set_config_input_filename("../../sample_files/no_such_file");
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT( monitor.get_cerr_output() == err_msg("config_input_filename", "../../sample_files/no_such_file", tmp_var->get_config_input_filename().c_str()));

  // should be able to set the value of 'config_input_filename' to a blank one
  tmp_var->set_config_input_filename("");
  CPPUNIT_ASSERT( "" == tmp_var->get_config_input_filename() );
}

void io_variables_test::test_load_puzzle_from_file() {
  // should be able to change the value of 'load_puzzle_from_file'
  tmp_var->set_load_puzzle_from_file(false);
  CPPUNIT_ASSERT( false == tmp_var->get_load_puzzle_from_file() );
  tmp_var->set_load_puzzle_from_file(true);
  CPPUNIT_ASSERT( true == tmp_var->get_load_puzzle_from_file() );
}

void io_variables_test::test_puzzle_input_filename(){
  // should be able to change the value of 'puzzle_input_filename' to an existing file
  tmp_var->set_puzzle_input_filename("../../sample_files/test_puzzle_input.png");
  CPPUNIT_ASSERT( "../../sample_files/test_puzzle_input.png" == tmp_var->get_puzzle_input_filename() );

  // should ignore attempts to change the filename to a file that does not exist
  tmp_var->set_puzzle_input_filename("../../sample_files/no_such_file");
  CPPUNIT_ASSERT( "../../sample_files/test_puzzle_input.png" == tmp_var->get_puzzle_input_filename() );

  // should print an error with attempts to change the filename to a file that does not exist
  monitor.start_recording_cerr();
    tmp_var->set_puzzle_input_filename("../../sample_files/no_such_file");
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT( monitor.get_cerr_output() == err_msg("puzzle_input_filename", "../../sample_files/no_such_file", tmp_var->get_puzzle_input_filename().c_str()));


  // should be able to set the value of 'puzzle_input_filename' to a blank one
  tmp_var->set_puzzle_input_filename("");
  CPPUNIT_ASSERT( "" == tmp_var->get_puzzle_input_filename() );
}

void io_variables_test::test_loaded_image() {
   // should be load and receive an opencv Mat
  Mat image(5,5, CV_32F, Scalar(5)); 
  tmp_var->set_loaded_image(image);
  CPPUNIT_ASSERT( 25 == image.dot(image)/25 ); // this test could be better
}

void io_variables_test::test_load_results_from_file() {
  // should be able to change the value of 'load_results_from_file'
  tmp_var->set_load_results_from_file(false);
  CPPUNIT_ASSERT( false == tmp_var->get_load_results_from_file() );
  tmp_var->set_load_results_from_file(true);
  CPPUNIT_ASSERT( true == tmp_var->get_load_results_from_file() );
}

void io_variables_test::test_results_input_filename(){
  // should be able to change the value of 'load_results_filename' to an existing file
  tmp_var->set_results_input_filename("../../sample_files/test_load_results.dat");
  CPPUNIT_ASSERT( "../../sample_files/test_load_results.dat" == tmp_var->get_results_input_filename() );

  // should ignore attempts to change the filename to a file that does not exist
  tmp_var->set_results_input_filename("../../sample_files/no_such_file");
  CPPUNIT_ASSERT( "../../sample_files/test_load_results.dat" == tmp_var->get_results_input_filename() );

  // should print an error with attempts to change the filename to a file that does not exist
  monitor.start_recording_cerr();
    tmp_var->set_results_input_filename("../../sample_files/no_such_file");
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT( monitor.get_cerr_output() == err_msg("results_input_filename", "../../sample_files/no_such_file", tmp_var->get_results_input_filename().c_str()));


  // should be able to set the value of 'load_results_filename' to a blank one
  tmp_var->set_results_input_filename("");
  CPPUNIT_ASSERT( "" == tmp_var->get_results_input_filename() );
}

void io_variables_test::test_loaded_results() {
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
  tmp_var->add_to_loaded_results(tmp_result_list);
  CPPUNIT_ASSERT(tmp_result_list == tmp_var->get_loaded_results());
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
  monitor.start_recording_cerr();
    tmp_var->set_number_of_final_results(-1);
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT( monitor.get_cerr_output() == err_msg("number_of_final_results", -1, 10) );
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
  monitor.start_recording_cerr();
    tmp_var->set_weighting("test", -1);
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT( monitor.get_cerr_output() == err_msg("pattern_weighting", "test", -1, 2));
}

void io_variables_test::test_show_text_results() {
  // should be able to change the value of 'show_text_results'
  tmp_var->set_show_text_results(false);
  CPPUNIT_ASSERT( false == tmp_var->get_show_text_results() );
  tmp_var->set_show_text_results(true);
  CPPUNIT_ASSERT( true == tmp_var->get_show_text_results() );
}

void io_variables_test::test_make_text_verbose() {
  // should be able to change the value of 'make_text_verbose'
  tmp_var->set_make_text_verbose(false);
  CPPUNIT_ASSERT( false == tmp_var->get_make_text_verbose() );
  tmp_var->set_make_text_verbose(true);
  CPPUNIT_ASSERT( true == tmp_var->get_make_text_verbose() );
}

void io_variables_test::test_save_text_to_file() {
  // should be able to change the value of 'save_text_to_file'
  tmp_var->set_save_text_to_file(false);
  CPPUNIT_ASSERT( false == tmp_var->get_save_text_to_file() );
  tmp_var->set_save_text_to_file(true);
  CPPUNIT_ASSERT( true == tmp_var->get_save_text_to_file() );
}

void io_variables_test::test_text_output_filename(){
  // should be able to change the value of 'text_output_filename'
  string file = "new_file";
  tmp_var->set_text_output_filename(file);
  CPPUNIT_ASSERT( file == tmp_var->get_text_output_filename() );

  // should not overwrite existing files
  file = "../../sample_files/test_text_output.txt";
  string new_file = "../../sample_files/test_text_output_1.txt";
  monitor.start_recording_cerr();
    tmp_var->set_text_output_filename(file);
  monitor.stop_recording_cerr();
  
 // CPPUNIT_ASSERT( monitor.get_cerr_output() == err_msg("text_output_filename", file.c_str(), new_file.c_str()) );

 // should increment names by one
  file = "../../sample_files/test_text_output_hi_1.txt";
  new_file = "../../sample_files/test_text_output_hi_2.txt";
  monitor.start_recording_cerr();
    tmp_var->set_text_output_filename(file);
  monitor.stop_recording_cerr();
 CPPUNIT_ASSERT( monitor.get_cerr_output() == err_msg("text_output_filename", file.c_str(), new_file.c_str()) );
}

void io_variables_test::test_show_graphic_results() {
  // should be able to change the value of 'show_graphic_results'
  tmp_var->set_show_graphic_results(false);
  CPPUNIT_ASSERT( false == tmp_var->get_show_graphic_results() );
  tmp_var->set_show_graphic_results(true);
  CPPUNIT_ASSERT( true == tmp_var->get_show_graphic_results() );
}

void io_variables_test::test_make_graphic_verbose() {
  // should be able to change the value of 'make_graphic_verbose'
  tmp_var->set_make_graphic_verbose(false);
  CPPUNIT_ASSERT( false == tmp_var->get_make_graphic_verbose() );
  tmp_var->set_make_graphic_verbose(true);
  CPPUNIT_ASSERT( true == tmp_var->get_make_graphic_verbose() );
}

void io_variables_test::test_save_graphic_to_file() {
  // should be able to change the value of 'save_graphic_to_file'
  tmp_var->set_save_graphic_to_file(false);
  CPPUNIT_ASSERT( false == tmp_var->get_save_graphic_to_file() );
  tmp_var->set_save_graphic_to_file(true);
  CPPUNIT_ASSERT( true == tmp_var->get_save_graphic_to_file() );
}

void io_variables_test::test_graphic_output_filename(){
  // should be able to change the value of 'graphic_output_filename'
  string file = "new_file";
  tmp_var->set_graphic_output_filename(file);
  CPPUNIT_ASSERT( file == tmp_var->get_graphic_output_filename() );

  // should not overwrite existing files
  file = "../../sample_files/test_graphic_output.png";
  string new_file = "../../sample_files/test_graphic_output_1.png";
  monitor.start_recording_cerr();
    tmp_var->set_graphic_output_filename(file);
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT( monitor.get_cerr_output() == err_msg("graphic_output_filename", file.c_str(), new_file.c_str()) );

 // should increment names by one
  file = "../../sample_files/test_graphic_output_hi_1.png";
  new_file = "../../sample_files/test_graphic_output_hi_2.png";
  monitor.start_recording_cerr();
    tmp_var->set_graphic_output_filename(file);
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT( monitor.get_cerr_output() == err_msg("graphic_output_filename", file.c_str(), new_file.c_str()) );
}

void io_variables_test::test_save_results_to_file() {
  // should be able to change the value of 'save_results_to_file'
  tmp_var->set_save_results_to_file(false);
  CPPUNIT_ASSERT( false == tmp_var->get_save_results_to_file() );
  tmp_var->set_save_results_to_file(true);
  CPPUNIT_ASSERT( true == tmp_var->get_save_results_to_file() );
}

void io_variables_test::test_results_output_filename() {
   // should be able to change the value of 'results_output_filename'
  string file = "new_file";
  tmp_var->set_results_output_filename(file);
  CPPUNIT_ASSERT( file == tmp_var->get_results_output_filename() );

  // should not overwrite existing files
  file = "../../sample_files/test_results_output.dat";
  string new_file = "../../sample_files/test_results_output_1.dat";
  monitor.start_recording_cerr();
    tmp_var->set_results_output_filename(file);
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT( monitor.get_cerr_output() == err_msg("results_output_filename", file.c_str(), new_file.c_str()) );

 // should increment names by one
  file = "../../sample_files/test_results_output_hi_1.dat";
  new_file = "../../sample_files/test_results_output_hi_2.dat";
  monitor.start_recording_cerr();
    tmp_var->set_results_output_filename(file);
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT( monitor.get_cerr_output() == err_msg("results_output_filename", file.c_str(), new_file.c_str()) );
}

// test the construction of the 'IO_Variables' class
void io_variables_test::test_constructor() {
  // defaults should be as here for the various results
  CPPUNIT_ASSERT( 3 == tmp_var->get_number_of_final_results() );    // should be able to see top 3 results

  CPPUNIT_ASSERT( 0 == tmp_var->get_weighting("anything") );        // weighting should be empty at initialisation

  CPPUNIT_ASSERT( true == tmp_var->get_show_text_results() );       // should print results by default
  CPPUNIT_ASSERT( false == tmp_var->get_make_text_verbose() );      // should not print all results by default

  CPPUNIT_ASSERT( true == tmp_var->get_show_graphic_results() );         // should diplay results by default
  CPPUNIT_ASSERT( false == tmp_var->get_make_graphic_verbose() );        // should not display all results by default
  CPPUNIT_ASSERT( false == tmp_var->get_save_text_to_file() );     // should not save print output to file by default
  CPPUNIT_ASSERT( true == tmp_var->get_load_puzzle_from_file() );   // should try to load puzzle from file by default
  CPPUNIT_ASSERT( false == tmp_var->get_load_config_from_file() );  // should not try to load configuration from file by default
  CPPUNIT_ASSERT( false == tmp_var->get_load_results_from_file() ); // should not try to load results from file by default
  CPPUNIT_ASSERT( "" == tmp_var->get_text_output_filename() );     //*
  CPPUNIT_ASSERT( "" == tmp_var->get_graphic_output_filename() );   //*
  CPPUNIT_ASSERT( "" == tmp_var->get_puzzle_input_filename() );     //*** all filenames should be blank at initialisation
  CPPUNIT_ASSERT( "" == tmp_var->get_config_input_filename() );           //*
  CPPUNIT_ASSERT( "" == tmp_var->get_results_input_filename() );     //*
}

int main(int argc, char* argv[]) {
  Test *suite = TestFactoryRegistry::getRegistry().makeTest();

  TextUi::TestRunner runner;
  runner.addTest( suite );

  runner.setOutputter( new CompilerOutputter(&runner.result(),cerr));

  bool wasSucessful = runner.run();
  return wasSucessful ? 0 : 1;
}
