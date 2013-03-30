cmake_minimum_required (VERSION 2.6) 


file(GLOB Engine_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)

add_library(Engine ${Engine_SRC})

target_link_libraries(Engine GLFW glew OpenGLHelpers Base)

set_target_properties(Engine
  PROPERTIES
  FOLDER "Libraries")