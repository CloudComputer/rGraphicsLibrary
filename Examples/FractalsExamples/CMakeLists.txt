cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

add_executable(FractalsExamples FractalsExamples.cpp)


target_link_libraries(FractalsExamples Fractals SuperEngine glew BoundingGeometry)

set_target_properties(FractalsExamples
  PROPERTIES
  FOLDER "Examples")
  

