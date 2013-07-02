cmake_minimum_required (VERSION 2.6) 

add_executable(PointRenderer PointRenderer.cpp)


target_link_libraries(PointRenderer  Volume Mesh Clustering IMHelpers GLFW glew OpenGLHelpers Util Volume Math SuperEngine glew BoundingGeometry CSG)

set_target_properties(PointRenderer
  PROPERTIES
  FOLDER "Examples")
  

