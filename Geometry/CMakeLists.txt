cmake_minimum_required (VERSION 2.6) 

include_directories("${PROJECT_SOURCE_DIR}/ext/")
include_directories(${PROJECT_SOURCE_DIR} )

set(Geometry_SRC BoundingVolume.cpp Mesh.cpp)
set(Geometry_H   BoundingVolume.h Mesh.h)

add_library(Geometry ${Geometry_SRC} ${Geometry_H})

set_target_properties(Geometry
  PROPERTIES
  FOLDER "Libraries")