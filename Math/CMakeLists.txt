cmake_minimum_required (VERSION 2.6) 

include_directories(${PROJECT_SOURCE_DIR}"/ext/")
include_directories(${PROJECT_SOURCE_DIR} )

file(GLOB Math_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)

add_library(Math ${Math_SRC})

set_target_properties(Math
  PROPERTIES
  FOLDER "Libraries")