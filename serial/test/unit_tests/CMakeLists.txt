add_executable(io_input_unit_test io_input_unit_test.cpp)
target_link_libraries(io_input_unit_test ${opencv2_libraries} ${CPPUNIT_LIBRARY} whereswally_io whereswally_framework)
add_test(io_input_unit_test io_input_unit_test)

