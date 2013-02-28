cmake_minimum_required (VERSION 2.6) 


file(GLOB Image_SRC
    "*.h"
    "*.hpp"
    "*.cpp"
)

add_library(Image ${Image_SRC})


target_link_libraries(Image Devil ilu ilut)

set_target_properties(Image
  PROPERTIES
  FOLDER "Libraries")