cmake_minimum_required (VERSION 2.6) 


file(GLOB IM_HLP_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)

add_library(IMHelpers ${IM_HLP_SRC})
target_link_libraries(IMHelpers IM OpenGLHelpers)

set_target_properties(IMHelpers
  PROPERTIES
  FOLDER "Libraries")