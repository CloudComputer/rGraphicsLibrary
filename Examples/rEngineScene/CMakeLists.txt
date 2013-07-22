cmake_minimum_required (VERSION 2.6) 


add_executable(rEngineScene rEngineScene.cpp)


target_link_libraries(rEngineScene rEngine)

set_target_properties(rEngineScene
  PROPERTIES
  FOLDER "Examples")
  

