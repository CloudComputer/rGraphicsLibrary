cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(SimpleViwer main.cpp)


target_link_libraries(SimpleViwer GLFW glew OpenGLHelpers Util Geometry OpenEXRHelpers)

set_target_properties(SimpleViwer
  PROPERTIES
  FOLDER "Examples/OpenEXRExamples")
  

