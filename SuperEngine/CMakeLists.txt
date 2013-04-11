cmake_minimum_required (VERSION 2.6) 


file(GLOB SuperEngine_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)

add_library(SuperEngine ${SuperEngine_SRC})

target_link_libraries(SuperEngine Engine GLFW glew OpenGLHelpers Base)

set_target_properties(SuperEngine
  PROPERTIES
  FOLDER "Libraries")