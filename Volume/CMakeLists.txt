 cmake_minimum_required (VERSION 2.6) 

 file(GLOB Volume_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)

add_library(Volume ${Volume_SRC})

target_link_libraries(Volume BoundingGeometry)

set_target_properties(Volume
  PROPERTIES
  FOLDER "Libraries")