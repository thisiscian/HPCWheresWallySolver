#include "ww_ut_output.h"
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace std;
using namespace CppUnit;

CPPUNIT_TEST_SUITE_REGISTRATION(output_test);

void output_test::setUp(){
  var = new IO_Variables();
  output = new Input(var);
  monitor.silence();
}

void output_test::tearDown(){
  monitor.unsilence();
  delete output;
  delete var;
}

// test the construction of the 'Input' class
void output_test::test_constructor() {
 CPPUNIT_ASSERT(var == output->variables);  // class variable  "variables" should be changed to output variable
}
int main(int argc, char* argv[]) {
  Test *suite = TestFactoryRegistry::getRegistry().makeTest();

  TextUi::TestRunner runner;
  runner.addTest( suite );

  runner.setOutputter( new CompilerOutputter(&runner.result(),cerr));

  bool wasSucessful = runner.run();
  return wasSucessful ? 0 : 1;
}
