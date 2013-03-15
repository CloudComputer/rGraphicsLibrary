cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(Cloth Cloth.cpp)


target_link_libraries(Cloth GLFW glew OpenGLHelpers Util Physics)

set_target_properties(Cloth
  PROPERTIES
  FOLDER "Examples")
  