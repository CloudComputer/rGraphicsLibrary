cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(MeshExample MeshExample.cpp)


target_link_libraries(MeshExample GLFW glew OpenGLHelpers Util Mesh)

set_target_properties(MeshExample
  PROPERTIES
  FOLDER "Examples")
  

