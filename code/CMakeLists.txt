cmake_minimum_required (VERSION 2.6)
project (HPCWheresWallySolver)
set (wheres_wally_VERSION_MAJOR 0)
set (wheres_wally_VERSION_MINOR 9)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules")
message(${CMAKE_MODULE_PATH})

include(source)
#set(OpenCV_DIR /usr/share/opencv)
set(OpenCV_DIR /Home/s1256705/opencv-2.4.6.1/test)
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
  include_directories(${OpenCV_INCLUDE_DIRS})
endif(OpenCV_FOUND)

set(CMAKE_CXX_FLAGS "-g -fopenmp -Wall -Wno-varargs -I${PROJECT_SOURCE_DIR}/include/")

make_directory(bin)
add_subdirectory(source)

option (BUILD_BENCHMARKS "Build benchmarks" ON)
option (BUILD_TESTS "Build CppUnit test suites" ON)

if (BUILD_BENCHMARKS) 
	add_subdirectory(bench)
endif()

if (BUILD_TESTS)
  enable_testing()
##  include(CTest)
  find_package(CppUnit)
  if(CPPUNIT_FOUND)
    include_directories(${CPPUNIT_INCLUDE_DIR})
    add_subdirectory(test/unit_tests)
  else(CPPUNIT_FOUND)
    message("Error: could not find cppunit")
  endif(CPPUNIT_FOUND)
endif (BUILD_TESTS)


