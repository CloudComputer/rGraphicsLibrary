cmake_minimum_required (VERSION 2.6) 

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/ext/")
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR} )

set(EulerianFluid_SRC EulerianFluid.cpp )
set(EulerianFluid_H   EulerianFluid.h )

add_library(FluidSimulator ${EulerianFluid_SRC} ${EulerianFluid_H})

set_target_properties(FluidSimulator
  PROPERTIES
  FOLDER "Libraries")