cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

#add_executable(FluidRenderer FluidRenderer/main.cpp FluidRenderer/glsl/fluid.vert FluidRenderer/glsl/fluid.frag)
add_executable(LevelSet LevelSet/main.cpp LevelSet/glsl/levelset.vert LevelSet/glsl/levelset.frag)
#add_executable(Shaders Shaders/main.cpp Shaders/glsl/shaders.vert Shaders/glsl/shaders.frag)


#target_link_libraries(FluidRenderer GLFW glew OpenGLHelpers Util)
target_link_libraries(LevelSet GLFW glew OpenGLHelpers Util Volume Geometry)
#target_link_libraries(Shaders GLFW glew OpenGLHelpers Util)

#set_target_properties(FluidRenderer
#  PROPERTIES
#  FOLDER "Examples")
  
set_target_properties(LevelSet
  PROPERTIES
  FOLDER "Examples")
#set_target_properties(Shaders
#  PROPERTIES
#  FOLDER "Examples")

# ADD_CUSTOM_COMMAND(TARGET Shaders
				   # POST_BUILD
				   # COMMAND ${CMAKE_COMMAND} -E copy 
				   # ARGS ${CMAKE_CURRENT_LIST_DIR}/Shaders/glsl/shaders.vert
				        # ${CMAKE_CURRENT_BINARY_DIR}/glsl/shaders.vert )

# ADD_CUSTOM_COMMAND(TARGET Shaders
				   # POST_BUILD
				   # COMMAND ${CMAKE_COMMAND} -E copy 
				   # ARGS ${CMAKE_CURRENT_LIST_DIR}/Shaders/glsl/shaders.frag
				        # ${CMAKE_CURRENT_BINARY_DIR}/glsl/shaders.frag )

ADD_CUSTOM_COMMAND(TARGET LevelSet
				   POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy 
				   ARGS ${CMAKE_CURRENT_LIST_DIR}/LevelSet/glsl/levelset.vert
				        ${CMAKE_CURRENT_BINARY_DIR}/glsl/levelset.vert )

ADD_CUSTOM_COMMAND(TARGET LevelSet
				   POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy 
				   ARGS ${CMAKE_CURRENT_LIST_DIR}/LevelSet/glsl/levelset.frag
				        ${CMAKE_CURRENT_BINARY_DIR}/glsl/levelset.frag )

# ADD_CUSTOM_COMMAND(TARGET FluidRenderer
				   # POST_BUILD
				   # COMMAND ${CMAKE_COMMAND} -E copy 
				   # ARGS ${CMAKE_CURRENT_LIST_DIR}/FluidRenderer/glsl/fluid.vert
				        # ${CMAKE_CURRENT_BINARY_DIR}/glsl/fluid.vert )

# ADD_CUSTOM_COMMAND(TARGET FluidRenderer
				   # POST_BUILD
				   # COMMAND ${CMAKE_COMMAND} -E copy 
				   # ARGS ${CMAKE_CURRENT_LIST_DIR}/FluidRenderer/glsl/fluid.frag
				        # ${CMAKE_CURRENT_BINARY_DIR}/glsl/fluid.frag )