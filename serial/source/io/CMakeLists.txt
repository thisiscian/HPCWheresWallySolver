add_library(whereswally_io error_message.cpp io_variables.cpp input.cpp output.cpp io_control.cpp)
target_link_libraries(whereswally_io ${OpenCV_LIBS} whereswally_framework)
