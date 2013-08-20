#include "ww_ut_output.h"
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace std;
using namespace CppUnit;

CPPUNIT_TEST_SUITE_REGISTRATION(output_test);

void output_test::setUp(){
  var = new IO_Variables();
  output = new Output(var);
  final_results.clear();
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
  final_results.push_back(result1);
  final_results.push_back(result2);
  monitor.silence();
}

void output_test::tearDown(){
  monitor.unsilence();
  delete output;
  delete var;
}

void output_test::test_final_results() {
  // should be able to change the value of final_results
  output->set_final_results(final_results);
  CPPUNIT_ASSERT(final_results == output->get_final_results());
}

void output_test::test_show_text_results() {
  var->set_show_graphic_results(false);
  output->set_final_results(final_results);
  monitor.start_recording_cout();
    output->output();
  monitor.stop_recording_cout();
  stringstream text_msg;
  text_msg << "Found potential Wallys in the following locations:" << endl;
  text_msg << "\t \"black and red\": 100\% certain of a Wally at 10x12, size 10x10" << endl;
  text_msg << "\t \"stripes\": 10\% certain of a Wally at 200x200, size 300x300" << endl;
  text_msg << endl;

  CPPUNIT_ASSERT( monitor.get_cout_output() == text_msg.str() );
}

int main(int argc, char* argv[]) {
  Test *suite = TestFactoryRegistry::getRegistry().makeTest();

  TextUi::TestRunner runner;
  runner.addTest( suite );

  runner.setOutputter( new CompilerOutputter(&runner.result(),cerr));

  bool wasSucessful = runner.run();
  return wasSucessful ? 0 : 1;
}
