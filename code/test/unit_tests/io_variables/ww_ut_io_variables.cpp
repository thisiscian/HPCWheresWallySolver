#include "ww_ut_io_variables.h"

using namespace std;
using namespace CppUnit;
using namespace cv;

CPPUNIT_TEST_SUITE_REGISTRATION(io_variables_test);

void io_variables_test::setUp(){
  tmp_var = new IO_Variables();
  error = -1;
  monitor.silence();
}
void io_variables_test::tearDown(){
  delete tmp_var;
  monitor.unsilence();
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
  tmp_var->set_puzzle_input_filename("../../sample_files/test_puzzle_image.png");
  CPPUNIT_ASSERT( "../../sample_files/test_puzzle_image.png" == tmp_var->get_puzzle_input_filename() );

  // should ignore attempts to change the filename to a file that does not exist
  tmp_var->set_puzzle_input_filename("../../sample_files/no_such_file");
  CPPUNIT_ASSERT( "../../sample_files/test_puzzle_image.png" == tmp_var->get_puzzle_input_filename() );

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

// test the construction of the 'IO_Variables' class
void io_variables_test::test_constructor() {
  // defaults should be as here for the various results
  CPPUNIT_ASSERT( 3 == tmp_var->get_number_of_final_results() );    // should be able to see top 3 results

  CPPUNIT_ASSERT( true == tmp_var->get_show_text_results() );       // should print results by default
  CPPUNIT_ASSERT( false == tmp_var->get_make_text_verbose() );      // should not print all results by default

  CPPUNIT_ASSERT( true == tmp_var->get_show_graphic_results() );         // should diplay results by default
  CPPUNIT_ASSERT( false == tmp_var->get_make_graphic_verbose() );        // should not display all results by default
  CPPUNIT_ASSERT( true == tmp_var->get_load_puzzle_from_file() );   // should try to load puzzle from file by default
  CPPUNIT_ASSERT( "" == tmp_var->get_puzzle_input_filename() );     //*** all filenames should be blank at initialisation
}

int main(int argc, char* argv[]) {
  Test *suite = TestFactoryRegistry::getRegistry().makeTest();

  TextUi::TestRunner runner;
  runner.addTest( suite );

  runner.setOutputter( new CompilerOutputter(&runner.result(),cerr));

  bool wasSucessful = runner.run();
  return wasSucessful ? 0 : 1;
}
