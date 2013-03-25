cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(gmres main_gmres.cpp)


target_link_libraries(gmres Util Base)

set_target_properties(gmres
  PROPERTIES
  FOLDER "Examples")
  