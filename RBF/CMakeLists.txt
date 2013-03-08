cmake_minimum_required (VERSION 2.6) 

include_directories(${PROJECT_SOURCE_DIR}"/ext/")
include_directories(${PROJECT_SOURCE_DIR} )

file(GLOB RBF_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)

add_library(RBF ${RBF_SRC})

set_target_properties(RBF
  PROPERTIES
  FOLDER "Libraries")