cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(EnvironmentLighting main.cpp)


target_link_libraries(EnvironmentLighting GLFW glew OpenGLHelpers Util Mesh OpenEXRHelpers  Half Iex IlmImf IlmThread Imath zlib)

set_target_properties(EnvironmentLighting
  PROPERTIES
  FOLDER "Examples/OpenEXRExamples")
  

