cmake_minimum_required (VERSION 2.6) 

include_directories(${PROJECT_SOURCE_DIR}"/ext/")
include_directories(${PROJECT_SOURCE_DIR} )

set(Math_SRC Interpolator.cpp)
set(Math_H   Interpolator.h)

add_library(Math ${Math_SRC} ${Math_H})

set_target_properties(Math
  PROPERTIES
  FOLDER "Libraries")