add_library(find_regions find_regions.cpp)
target_link_libraries(find_regions ${OpenCV_LIBS})
add_library(pattern_redandwhite redandwhite.cpp)
target_link_libraries(pattern_redandwhite ${OpenCV_LIBS} find_regions)
