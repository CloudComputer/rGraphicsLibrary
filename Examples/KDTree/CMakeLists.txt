cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(KDTreeExample KDTreeExample.cpp)


target_link_libraries(KDTreeExample Util Base)

set_target_properties(KDTreeExample
  PROPERTIES
  FOLDER "Examples")
  