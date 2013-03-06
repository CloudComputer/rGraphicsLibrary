cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(KDTree KDTree.cpp)


target_link_libraries(KDTree Util Geometry)

set_target_properties(KDTree
  PROPERTIES
  FOLDER "Examples")
  