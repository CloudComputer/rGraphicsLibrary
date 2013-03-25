cmake_minimum_required (VERSION 2.6) 

set(Util_SRC StopClock.cpp Directory.cpp)

file(GLOB Util_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)



add_library(Util ${Util_SRC})

set_target_properties(Util
  PROPERTIES
  FOLDER "Libraries")