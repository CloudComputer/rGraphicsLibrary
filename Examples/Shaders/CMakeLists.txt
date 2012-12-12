cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(Shaders main.cpp)


target_link_libraries(Shaders GLFW glew OpenGLHelpers Util)

set_target_properties(Shaders
  PROPERTIES
  FOLDER "Examples")
  