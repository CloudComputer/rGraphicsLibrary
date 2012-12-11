cmake_minimum_required (VERSION 2.6) 

find_package(OpenGL REQUIRED)


set(OpenGLHelpers_SRC FBO.cpp Shader.cpp OpenGLInfo.cpp ColorCube.cpp AttribPusher.cpp)
set(OpenGLHelpers_H   FBO.h Shader.h OpenGLInfo.h ColorCube.h AttribPusher.h)

add_library(OpenGLHelpers ${OpenGLHelpers_SRC} ${OpenGLHelpers_H})
target_link_libraries(OpenGLHelpers glew)

set_target_properties(OpenGLHelpers
  PROPERTIES
  FOLDER "Libraries")