cmake_minimum_required (VERSION 2.6) 

add_executable(VolumeDimensionEstimator main.cpp)

target_link_libraries(VolumeDimensionEstimator Base)

set_target_properties(VolumeDimensionEstimator
  PROPERTIES
  FOLDER "Tools")
  