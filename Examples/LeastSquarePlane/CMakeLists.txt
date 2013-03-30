cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(LeastSquarePlane LeastSquarePlane.cpp)


target_link_libraries(LeastSquarePlane Math Util Engine glew BoundingGeometry)

set_target_properties(LeastSquarePlane
  PROPERTIES
  FOLDER "Examples")
  

