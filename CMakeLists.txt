cmake_minimum_required (VERSION 2.8.4) 
project (rGraphicsLibrary) 

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(BUILD_EXAMPLES "Build Examples" ON)
option(BUILD_OPENGL_HELPERS "Build Classes to handle some opengl features" ON)
option(BUILD_UNIT_TEST "Build tests" ON)
option(BUILD_GLEW "Build glew" ON)


include_directories("${PROJECT_SOURCE_DIR}/ext/")
include_directories(${PROJECT_SOURCE_DIR} )


add_subdirectory(FluidSimulator)
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