cmake_minimum_required (VERSION 2.6) 

add_executable(2DFractals 2DFractals.cpp)


target_link_libraries(2DFractals IMHelpers)

set_target_properties(2DFractals
  PROPERTIES
  FOLDER "Examples")
  

