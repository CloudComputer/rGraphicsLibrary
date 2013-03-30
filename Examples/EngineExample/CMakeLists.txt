cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(EngineExample EngineExample.cpp)


target_link_libraries(EngineExample Engine glew BoundingGeometry)

set_target_properties(EngineExample
  PROPERTIES
  FOLDER "Examples")
  

