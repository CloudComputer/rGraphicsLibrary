cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(FluidRenderer FluidRenderer.cpp)


target_link_libraries(FluidRenderer GLFW glew OpenGLHelpers Util)

set_target_properties(FluidRenderer
  PROPERTIES
  FOLDER "Examples")
  