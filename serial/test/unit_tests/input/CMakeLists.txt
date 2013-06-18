add_executable(ww_ut_input ww_ut_input.cpp)
target_link_libraries(ww_ut_input ${CPPUNIT_LIBRARY} ww_ut_general whereswally_io whereswally_framework)
add_test(test_input_class ww_ut_input)

