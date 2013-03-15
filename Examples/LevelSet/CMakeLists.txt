cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(LevelSet LevelSetExample.cpp)


target_link_libraries(LevelSet GLFW glew OpenGLHelpers Util Volume Mesh)

set_target_properties(LevelSet
  PROPERTIES
  FOLDER "Examples")
  

