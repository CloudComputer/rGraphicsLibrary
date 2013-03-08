cmake_minimum_required (VERSION 2.6) 




file(GLOB UMFPACK_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
    "*.c"
)

add_library(UMFPACK ${UMFPACK_SRC})

set_target_properties(UMFPACK
  PROPERTIES
  FOLDER "ext")