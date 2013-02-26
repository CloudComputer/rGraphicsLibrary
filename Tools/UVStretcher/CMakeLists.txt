cmake_minimum_required (VERSION 2.6) 

add_executable(UVStretcher UVStretcher.cpp)

target_link_libraries(UVStretcher Base)

set_target_properties(UVStretcher
  PROPERTIES
  FOLDER "Tools")
  