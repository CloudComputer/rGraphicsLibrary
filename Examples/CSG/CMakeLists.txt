cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(CSGExample CSGExample.cpp)


target_link_libraries(CSGExample GLFW glew OpenGLHelpers Util CSG)

set_target_properties(CSGExample
  PROPERTIES
  FOLDER "Examples")
  

