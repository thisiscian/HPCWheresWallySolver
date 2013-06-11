#ifndef io_input_unit_test
#define io_input_unit_test

  #include <string>
  #include <iostream>
  #include <cppunit/extensions/HelperMacros.h>
  #include "../../headers/io/input.h"

  class input_test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(input_test);
    CPPUNIT_TEST( test_constructor);
    CPPUNIT_TEST_SUITE_END();
    public:
      void setUp();
      void tearDown();
      void test_constructor();
  };

#endif
