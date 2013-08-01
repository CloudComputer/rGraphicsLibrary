cmake_minimum_required (VERSION 2.6) 


file(GLOB SuperEngine_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)

add_library(rEngine ${SuperEngine_SRC})

target_link_libraries(rEngine glfw3 glew TinyXML2 Util OpenGLHelpers IMHelpers Mesh CSG Math Clustering)

set_target_properties(rEngine
  PROPERTIES
  FOLDER "Libraries")