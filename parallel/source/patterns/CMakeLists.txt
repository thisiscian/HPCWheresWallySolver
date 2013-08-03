set_property(GLOBAL APPEND PROPERTY SOURCE ${CMAKE_CURRENT_LIST_DIR}/pattern_tools.cpp)
set_property(GLOBAL APPEND PROPERTY SOURCE ${CMAKE_CURRENT_LIST_DIR}/redandwhite.cpp)
set_property(GLOBAL APPEND PROPERTY SOURCE ${CMAKE_CURRENT_LIST_DIR}/bluetrousers.cpp)
set_property(GLOBAL APPEND PROPERTY SOURCE ${CMAKE_CURRENT_LIST_DIR}/findglasses.cpp)
set_property(GLOBAL APPEND PROPERTY SOURCE ${CMAKE_CURRENT_LIST_DIR}/findfeatures.cpp)

add_library(pattern_tools pattern_tools.cpp)
target_link_libraries(pattern_tools ${OpenCV_LIBS})
