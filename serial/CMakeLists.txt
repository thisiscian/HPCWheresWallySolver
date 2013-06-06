cmake_minimum_required (VERSION 2.6)
project (HPCWheresWallySolver)
set (wheres_wally_VERSION_MAJOR 0)
set (wheres_wally_VERSION_MINOR 0)

add_subdirectory (src)

option (BUILD_TESTS "Build CppUnit test suites" ON)
if (BUILD_TESTS)
  include_directories("${PROJECT_SOURCE_DIR}/test")
  include_directories("${PROJECT_SOURCE_DIR}/test/unit_tests")
  add_subdirectory(test)
  add_subdirectory(test/unit_tests)
endif (BUILD_TESTS)

