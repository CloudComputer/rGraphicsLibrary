cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

add_definitions( -DGLEW_STATIC)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})


#add_subdirectory(FluidRenderer)
add_subdirectory(LevelSet)
add_subdirectory(Mesh)
add_subdirectory(GameOfLife)
add_subdirectory(Shaders)
add_subdirectory(UltrasoundSurfaceExtaction)
add_subdirectory(OpenEXR)
add_subdirectory(Cloth)