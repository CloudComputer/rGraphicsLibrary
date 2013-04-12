cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(XMLExample XMLExample.cpp)


target_link_libraries(XMLExample Base)

set_target_properties(XMLExample
  PROPERTIES
  FOLDER "Examples")
  

