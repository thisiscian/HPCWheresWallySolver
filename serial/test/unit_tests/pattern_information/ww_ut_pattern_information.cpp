#include "ww_ut_pattern_information.h"
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace std;
using namespace CppUnit;

CPPUNIT_TEST_SUITE_REGISTRATION(pattern_information_test);

void pattern_information_test::setUp(){
  tmp_patt = new Pattern_Information();
  monitor.silence();
}
void pattern_information_test::tearDown(){
  delete tmp_patt;
  monitor.unsilence();
}

void pattern_information_test::test_name() {
  // should be able to change the value of 'name'
  tmp_patt->set_name("hello");
  CPPUNIT_ASSERT( "hello" == tmp_patt->get_name() );
}

void pattern_information_test::test_description() {
  // should be able to change the value of 'description'
  tmp_patt->set_description("very good");
  CPPUNIT_ASSERT( "very good" == tmp_patt->get_description() );
}

void pattern_information_test::test_confidence() {
  // should be able to change the value of 'confidence'
  tmp_patt->set_confidence(4.0);
  CPPUNIT_ASSERT( 4.0 == tmp_patt->get_confidence() );

  // should ignore attempts to set 'confidence' to values less than 0 and print errors
  monitor.start_recording_cerr();
    tmp_patt->set_confidence(-1.0);
  monitor.stop_recording_cerr();
  CPPUNIT_ASSERT_MESSAGE( "failed <0 test", 4.0 == tmp_patt->get_confidence() );
  CPPUNIT_ASSERT( "Error: Confidence values should be non-negative\n" == monitor.get_cerr_output() );
}

void pattern_information_test::test_relational_operators() {
  tmp_patt->set_name("hello");
  tmp_patt->set_description("very good");
  tmp_patt->set_confidence(4.0);
  Pattern_Information same_patt = *tmp_patt;
  Pattern_Information diff_patt;
  diff_patt.set_name("bye");
  diff_patt.set_description("quite bad");
  diff_patt.set_confidence(2.3);

  // should be able to make sane relational operations
  CPPUNIT_ASSERT( *tmp_patt == same_patt);
  CPPUNIT_ASSERT( *tmp_patt != diff_patt);
}

// test the construction of the 'IO_Variables' class
void pattern_information_test::test_constructor() {
  // expect defaults to be the following
  CPPUNIT_ASSERT(tmp_patt->get_name() == ""); 
  CPPUNIT_ASSERT(tmp_patt->get_description() == ""); 
  CPPUNIT_ASSERT(tmp_patt->get_confidence() == 1.0); 
}

int main(int argc, char* argv[]) {
  Test *suite = TestFactoryRegistry::getRegistry().makeTest();

  TextUi::TestRunner runner;
  runner.addTest( suite );

  runner.setOutputter( new CompilerOutputter(&runner.result(),cerr));

  bool wasSucessful = runner.run();
  return wasSucessful ? 0 : 1;
}
