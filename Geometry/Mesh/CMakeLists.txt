cmake_minimum_required (VERSION 2.6) 

include_directories("${PROJECT_SOURCE_DIR}/ext/")
include_directories(${PROJECT_SOURCE_DIR} )


file(GLOB Mesh_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)


add_library(Mesh ${Mesh_SRC})

target_link_libraries(Mesh Base BoundingGeometry)

set_target_properties(Mesh
  PROPERTIES
  FOLDER "Libraries/Geometry")