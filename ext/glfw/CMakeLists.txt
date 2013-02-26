file(GLOB GLFW_SOURCE
    "include/*.h"
    "include/*.hpp"
    "include/*.cpp"
    "include/*.c"
    # "src/*/*.h"
    # "src/*/*.hpp"
    # "src/*/*.cpp"
    # "src/*/*.c"
    "src/*.h"
    "src/*.hpp"
    "src/*.cpp"
    "src/*.c"
)



add_library(GLFW ${GLFW_SOURCE})

set_target_properties(GLFW
  PROPERTIES
  FOLDER "ext")