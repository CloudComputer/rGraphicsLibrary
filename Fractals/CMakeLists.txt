cmake_minimum_required (VERSION 2.6) 

include_directories("${PROJECT_SOURCE_DIR}/ext/")
include_directories(${PROJECT_SOURCE_DIR} )


file(GLOB Fractals_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)


add_library(Fractals ${Fractals_SRC})

target_link_libraries(Fractals Base Mesh BoundingGeometry)

set_target_properties(Fractals
  PROPERTIES
  FOLDER "Libraries/Geometry")