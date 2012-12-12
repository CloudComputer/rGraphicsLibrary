cmake_minimum_required (VERSION 2.6) 

set(OpenEXRHelpers_SRC Reader.cpp Texture.cpp)
set(OpenEXRHelpers_H   Reader.h Texture.h OpenEXRIncludes.h)

add_library(OpenEXRHelpers ${OpenEXRHelpers_SRC} ${OpenEXRHelpers_H})

target_link_libraries(OpenEXRHelpers Half Iex IlmImf IlmThread Imath)

set_target_properties(OpenEXRHelpers
  PROPERTIES
  FOLDER "Libraries")