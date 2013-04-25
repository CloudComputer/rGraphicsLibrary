cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(IMExample IMExample.cpp)


target_link_libraries(IMExample IMHelpers)

set_target_properties(IMExample
  PROPERTIES
  FOLDER "Examples")
  

