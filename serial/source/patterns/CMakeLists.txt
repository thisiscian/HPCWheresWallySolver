add_library(pattern_tools pattern_tools.cpp)
target_link_libraries(pattern_tools ${OpenCV_LIBS})

add_library(pattern_redandwhite redandwhite.cpp)
target_link_libraries(pattern_redandwhite ${OpenCV_LIBS} pattern_tools)

add_library(pattern_findglasses findglasses.cpp)
target_link_libraries(pattern_findglasses ${OpenCV_LIBS} pattern_tools)

add_library(pattern_findfeatures findfeatures.cpp)
target_link_libraries(pattern_findfeatures ${OpenCV_LIBS} pattern_tools)

add_library(whereswally_patterns)
target_link_libraries(whereswally_patterns ${OpenCV_LIBS} pattern_redandwhite pattern_findglasses pattern_findfeatures)
