include_directories("${PROJECT_SOURCE_DIR}/include")

add_subdirectory(framework)
add_subdirectory(io)
add_subdirectory(patterns)

add_executable(../bin/whereswallysolver main.cpp)
target_link_libraries(../bin/whereswallysolver whereswally_io whereswally_framework whereswally_patterns)
