cmake_minimum_required (VERSION 2.6) 

include_directories(${PROJECT_SOURCE_DIR}"/ext/")
include_directories(${PROJECT_SOURCE_DIR} )

file(GLOB Base_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)

add_library(Base ${Base_SRC} ${Base_H})

set_target_properties(Base
  PROPERTIES
  FOLDER "Base")