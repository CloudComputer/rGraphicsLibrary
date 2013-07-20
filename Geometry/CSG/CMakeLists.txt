cmake_minimum_required (VERSION 2.6) 

include_directories("${PROJECT_SOURCE_DIR}/ext/")
include_directories(${PROJECT_SOURCE_DIR} )


file(GLOB CSG_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)


add_library(CSG ${CSG_SRC})

target_link_libraries(CSG Volume BoundingGeometry RBF Base Mesh)

set_target_properties(CSG
  PROPERTIES
  FOLDER "Libraries/Geometry")