cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(RBFExample RBFExample.cpp)


target_link_libraries(RBFExample GLFW glew OpenGLHelpers Util CSG RBF)

set_target_properties(RBFExample
  PROPERTIES
  FOLDER "Examples")
  

