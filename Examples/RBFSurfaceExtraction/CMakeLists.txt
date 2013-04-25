cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(RBFSurfaceExtraction RBFSurfaceExtraction.cpp)


target_link_libraries(RBFSurfaceExtraction Image SuperEngine Util Mesh CSG RBF)

set_target_properties(RBFSurfaceExtraction
  PROPERTIES
  FOLDER "Examples")
  