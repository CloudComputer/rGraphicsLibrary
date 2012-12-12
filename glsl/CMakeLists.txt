cmake_minimum_required (VERSION 2.8.4) 

file(GLOB shaderfiles
    "*.vert"
    "*.frag"
)

add_library(GLSLfiles EXCLUDE_FROM_ALL ${shaderfiles} ) 

SET_TARGET_PROPERTIES(GLSLfiles PROPERTIES LINKER_LANGUAGE C)

set_target_properties(GLSLfiles
  PROPERTIES
  FOLDER "Examples")