cmake_minimum_required (VERSION 2.6) 


file(GLOB Physics_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)

add_library(Physics ${Physics_SRC})

set_target_properties(Physics
  PROPERTIES
  FOLDER "Libraries")