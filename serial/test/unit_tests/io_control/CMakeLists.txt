add_executable(ww_ut_io_control ww_ut_io_control.cpp)
target_link_libraries(ww_ut_io_control ${CPPUNIT_LIBRARY} ${OPENCV_LIBS} ww_ut_general whereswally_io whereswally_framework)
add_test(test_io_control_class ww_ut_io_control)

