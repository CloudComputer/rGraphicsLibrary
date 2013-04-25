cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(UltrasoundSurfacePoints UltrasoundSurfacePoints.cpp)


target_link_libraries(UltrasoundSurfacePoints Mesh Clustering Image GLFW glew OpenGLHelpers Util Volume Math SuperEngine glew BoundingGeometry)

set_target_properties(UltrasoundSurfacePoints
  PROPERTIES
  FOLDER "Examples")
  