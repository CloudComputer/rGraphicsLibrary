cmake_minimum_required (VERSION 2.6) 

include_directories("${PROJECT_SOURCE_DIR}/ext/")
include_directories(${PROJECT_SOURCE_DIR} )


file(GLOB Geometry_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)


add_library(Geometry ${Geometry_SRC})

set_target_properties(Geometry
  PROPERTIES
  FOLDER "Libraries")