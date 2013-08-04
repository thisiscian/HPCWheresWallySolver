add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/framework)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/io)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/patterns)


include_directories(${CMAKE_CURRENT_LIST_DIR}/../include)
get_property(SOURCE GLOBAL PROPERTY SOURCE)
add_library(whereswally ${SOURCE})
target_link_libraries(whereswally ${OpenCV_LIBS})

add_executable(../bin/whereswallysolver main.cpp)
target_link_libraries(../bin/whereswallysolver whereswally)
