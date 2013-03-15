cmake_minimum_required (VERSION 2.6) 

include_directories("${PROJECT_SOURCE_DIR}/ext/")
include_directories(${PROJECT_SOURCE_DIR} )


file(GLOB Clustering_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)


add_library(Clustering ${Clustering_SRC})

target_link_libraries(Clustering Base)

set_target_properties(Clustering
  PROPERTIES
  FOLDER "Libraries/Geometry")