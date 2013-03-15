cmake_minimum_required (VERSION 2.6) 

include_directories("${PROJECT_SOURCE_DIR}/ext/")
include_directories(${PROJECT_SOURCE_DIR} )


file(GLOB KDTree_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)


add_library(KDTree ${KDTree_SRC})

target_link_libraries(KDTree Base)

set_target_properties(KDTree
  PROPERTIES
  FOLDER "Libraries/Geometry")