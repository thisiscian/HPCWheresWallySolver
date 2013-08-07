include(framework)
include(io)
include(patterns)
add_subdirectory(framework)
add_subdirectory(io)
add_subdirectory(patterns)


include_directories(${CMAKE_CURRENT_LIST_DIR}/../include)
get_property(SOURCE GLOBAL PROPERTY SOURCE)

add_library(whereswally ${SOURCE})
target_link_libraries(whereswally ${OpenCV_LIBS})

add_executable(../bin/whereswallysolver main.cpp)
target_link_libraries(../bin/whereswallysolver whereswally)
