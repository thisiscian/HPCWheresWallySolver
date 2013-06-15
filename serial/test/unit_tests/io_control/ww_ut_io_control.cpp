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
  record_output.silence();
}

void input_test::tearDown(){
  record_output.unsilence();
  delete input;
  delete var;
}

// test the construction of the 'Input' class
void input_test::test_constructor() {
 CPPUNIT_ASSERT(var == input->variables);  // class variable  "variables" should be changed to input variable
}

int main(int argc, char* argv[]) {
  Test *suite = TestFactoryRegistry::getRegistry().makeTest();

  TextUi::TestRunner runner;
  runner.addTest( suite );

  runner.setOutputter( new CompilerOutputter(&runner.result(),cerr));

  bool wasSucessful = runner.run();
  return wasSucessful ? 0 : 1;
}
