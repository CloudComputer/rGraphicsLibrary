cmake_minimum_required (VERSION 2.6) 

find_package(OpenGL REQUIRED)


file(GLOB OpenGLHelpers_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)

add_library(OpenGLHelpers ${OpenGLHelpers_SRC})
target_link_libraries(OpenGLHelpers glew Util)

set_target_properties(OpenGLHelpers
  PROPERTIES
  FOLDER "Libraries")