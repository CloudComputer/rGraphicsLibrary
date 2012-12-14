cmake_minimum_required (VERSION 2.6) 

set(Util_SRC StopClock.cpp Directory.cpp)
set(Util_H   StopClock.h   Directory.h)

add_library(Util ${Util_SRC} ${Util_H})

set_target_properties(Util
  PROPERTIES
  FOLDER "Libraries")