add_executable(ww_ut_io_control ww_ut_io_control.cpp)
target_link_libraries(ww_ut_io_control ${CPPUNIT_LIBRARY} ww_ut_general whereswally)
add_test(test_io_control_class ww_ut_io_control)
