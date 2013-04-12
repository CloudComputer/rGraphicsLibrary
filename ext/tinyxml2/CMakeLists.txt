cmake_minimum_required (VERSION 2.6) 



file(GLOB TinyXML2_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)


add_library(TinyXML2 ${TinyXML2_SRC})

target_link_libraries(TinyXML2)

set_target_properties(TinyXML2
  PROPERTIES
  FOLDER "ext")