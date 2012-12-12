cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(Mesh main.cpp)


target_link_libraries(Mesh GLFW glew OpenGLHelpers Util Geometry)

set_target_properties(Mesh
  PROPERTIES
  FOLDER "Examples")
  

