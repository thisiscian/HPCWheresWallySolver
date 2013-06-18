add_executable(ww_ut_output ww_ut_output.cpp)
target_link_libraries(ww_ut_output ${CPPUNIT_LIBRARY} ww_ut_general whereswally_io whereswally_framework)
add_test(test_output_class ww_ut_output)

