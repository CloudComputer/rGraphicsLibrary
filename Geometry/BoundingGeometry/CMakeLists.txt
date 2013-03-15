cmake_minimum_required (VERSION 2.6) 

include_directories("${PROJECT_SOURCE_DIR}/ext/")
include_directories(${PROJECT_SOURCE_DIR} )


file(GLOB BoundingGeometry_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)


add_library(BoundingGeometry ${BoundingGeometry_SRC})

target_link_libraries(BoundingGeometry Base)

set_target_properties(BoundingGeometry
  PROPERTIES
  FOLDER "Libraries/Geometry")