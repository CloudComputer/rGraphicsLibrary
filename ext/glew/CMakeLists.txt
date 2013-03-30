cmake_minimum_required (VERSION 2.8.4) 


FIND_PACKAGE(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )



add_library(glew glew.c visualinfo.c)
target_link_libraries(glew ${OPENGL_LIBRARIES})
set_target_properties(glew
  PROPERTIES
  FOLDER "ext/glew")



add_executable(glewinfo glewinfo.c)
add_executable(visualinfo visualinfo.c)

target_link_libraries(glewinfo ${OPENGL_LIBRARIES} glew)
target_link_libraries(visualinfo ${OPENGL_LIBRARIES} glew)

set_target_properties(glewinfo
  PROPERTIES
  FOLDER "ext/glew")
set_target_properties(visualinfo
  PROPERTIES
  FOLDER "ext/glew")