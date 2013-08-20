#include "ww_ut_search_pattern.h"
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace std;
using namespace CppUnit;

CPPUNIT_TEST_SUITE_REGISTRATION(search_pattern_test);

void search_pattern_test::setUp(){
  tmp_search = new Search_Pattern;
  monitor.silence();
}
void search_pattern_test::tearDown(){
  delete tmp_search;
  monitor.unsilence();
}

void search_pattern_test::test_pattern_information() {
  // should be able to return pattern_information
  Pattern_Information tmp1 = tmp_search->get_pattern_information();
  Pattern_Information tmp2;
  CPPUNIT_ASSERT( tmp1 == tmp2 );
}

void search_pattern_test::test_result_relational_operators() {
  result.info.set_name("hello");
  result.info.set_description("very good");
  result.info.set_confidence(4.0);
  result.wally_location[0] = 10;
  result.wally_location[1] = 20;
  result.scale[0] = 10;
  result.scale[1] = 10;
  result.certainty = 0.9;

  Pattern_Result result2 = result;
  Pattern_Result result3;

  result3.info.set_name("bye");
  result3.info.set_description("quite bad");
  result3.info.set_confidence(2.3);
  result3.wally_location[0] = 80;
  result3.wally_location[1] = 2;
  result3.scale[0] = 5;
  result3.scale[1] = 100;
  result3.certainty = 0.1;
  
  // should be able to make sane relational operations
  CPPUNIT_ASSERT( result == result2);
  CPPUNIT_ASSERT( result != result3);
}

int main(int argc, char* argv[]) {
  Test *suite = TestFactoryRegistry::getRegistry().makeTest();

  TextUi::TestRunner runner;
  runner.addTest( suite );

  runner.setOutputter( new CompilerOutputter(&runner.result(),cerr));

  bool wasSucessful = runner.run();
  return wasSucessful ? 0 : 1;
}
