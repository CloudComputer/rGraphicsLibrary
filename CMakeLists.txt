cmake_minimum_required (VERSION 2.8.4) 
project (rGraphicsLibrary) 

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


SET(EXR_IMAGES_FOLDER "${PROJECT_SOURCE_DIR}/EXR_Images" CACHE PATH "EXR_IMAGES_FOLDER")
SET(GLSL_FOLDER "${PROJECT_SOURCE_DIR}/glsl" CACHE PATH "GLSL_FOLDER")
option(BUILD_EXAMPLES "Build Examples" ON)
option(BUILD_OPENGL_HELPERS "Build Classes to handle some opengl features" ON)
option(BUILD_UNIT_TEST "Build tests" ON)
option(BUILD_GLEW "Build glew" ON)

#SET(OPENEXRLIBS "Half Iex IlmImf IlmThread Imath")

include_directories("${PROJECT_SOURCE_DIR}/ext/")
include_directories("${PROJECT_SOURCE_DIR}/ext/openEXR/include/")
include_directories(${PROJECT_SOURCE_DIR} )

link_directories("${PROJECT_SOURCE_DIR}/ext/openEXR/msvc10/" )

add_definitions(-DGLSL_DIR="${GLSL_FOLDER}")
add_definitions(-DEXR_IMAGES_DIR="${EXR_IMAGES_FOLDER}")


add_subdirectory(glsl)


add_subdirectory(FluidSimulator)
add_subdirectory(OpenEXRHelpers)
add_subdirectory(Geometry)
add_subdirectory(Math)
add_subdirectory(Volume)
add_subdirectory(Util)

if(BUILD_GLEW)
	add_subdirectory(glew)
endif()

if(BUILD_OPENGL_HELPERS)
	add_subdirectory(OpenGLHelpers)
endif()


if(BUILD_UNIT_TEST)
	add_subdirectory(UnitTests)
endif()


if(BUILD_EXAMPLES)
	add_subdirectory(Examples)
endif()