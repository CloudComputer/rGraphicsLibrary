cmake_minimum_required (VERSION 2.6) 

include_directories(${PROJECT_SOURCE_DIR}"/ext/")
include_directories(${PROJECT_SOURCE_DIR} )

set(Util_SRC StopClock.cpp)
set(Util_H   StopClock.h)

add_library(Util ${Util_SRC} ${Util_H})

set_target_properties(Util
  PROPERTIES
  FOLDER "Libraries")