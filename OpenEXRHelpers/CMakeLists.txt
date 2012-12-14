cmake_minimum_required (VERSION 2.8.4) 

file(GLOB OpenEXRHelpers_source
    "*.h"
    "*.hpp"
    "*.cpp"
)

add_library(OpenEXRHelpers ${OpenEXRHelpers_source})

target_link_libraries(OpenEXRHelpers Half Iex IlmImf IlmThread Imath)

set_target_properties(OpenEXRHelpers
  PROPERTIES
  FOLDER "Libraries")