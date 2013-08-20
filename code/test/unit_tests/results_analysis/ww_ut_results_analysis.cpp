#include "ww_ut_results_analysis.h"
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace std;
using namespace CppUnit;

CPPUNIT_TEST_SUITE_REGISTRATION(results_analysis_test);

void results_analysis_test::setUp(){
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
  Pattern_Result result3;
    result3.info.set_name("wally"); 
    result3.info.set_description("fine");
    result3.info.set_confidence(0.5);
    result3.wally_location[0] = 40; 
    result3.wally_location[1] = 50; 
    result3.scale[0] = 30;
    result3.scale[1] = 28;
    result3.certainty = 0.5;
  input_results.push_back(result1);
  input_results.push_back(result2);
  input_results.push_back(result3);

  ra = new Results_Analysis;
  monitor.silence();
}
void results_analysis_test::tearDown() {
  delete ra;
  input_results.clear();
  output_results.clear();
  monitor.unsilence();
}

void results_analysis_test::test_calculate_final_results() {
  int final_count = 1;
  
  // should be able to get 'best' result
  output_results.push_back(input_results[0]);
  ra->calculate_final_results(final_count,input_results);
  CPPUNIT_ASSERT(output_results == ra->get_final_results() );

  //should be able to get 'best' results, even if they are out of order
  final_count = 2;
  output_results.push_back(input_results[2]);
  ra->calculate_final_results(final_count,input_results);
  CPPUNIT_ASSERT(output_results == ra->get_final_results() );
}

void results_analysis_test::test_final_results() {
  ra->add_to_final_results(input_results);
  CPPUNIT_ASSERT( ra->get_final_results() == input_results );
}

int main(int argc, char* argv[]) {
  Test *suite = TestFactoryRegistry::getRegistry().makeTest();

  TextUi::TestRunner runner;
  runner.addTest( suite );

  runner.setOutputter( new CompilerOutputter(&runner.result(),cerr));

  bool wasSucessful = runner.run();
  return wasSucessful ? 0 : 1;
}
