cmake_minimum_required (VERSION 2.6) 

add_executable(ScatteredPoints ScatteredPoints.cpp)


target_link_libraries(ScatteredPoints Util Volume Math SuperEngine BoundingGeometry CSG)

set_target_properties(ScatteredPoints
  PROPERTIES
  FOLDER "Examples")
  

