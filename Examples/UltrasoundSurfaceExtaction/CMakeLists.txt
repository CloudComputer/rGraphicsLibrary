cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(UltrasoundSurfaceExtaction UltrasoundSurfaceExtaction.cpp)


target_link_libraries(UltrasoundSurfaceExtaction Image GLFW glew OpenGLHelpers Util Volume Geometry UMFPACK)

set_target_properties(UltrasoundSurfaceExtaction
  PROPERTIES
  FOLDER "Examples")
  