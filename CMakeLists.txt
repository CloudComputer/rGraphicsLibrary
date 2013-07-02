cmake_minimum_required (VERSION 2.8.4) 
project (rGraphicsLibrary) 

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


SET(OUTDIR "${CMAKE_BINARY_DIR}/bin")
SET(EXECUTABLE_OUTPUT_PATH ${OUTDIR})
SET(LIBRARY_OUTPUT_PATH ${OUTDIR})

SET(EXR_IMAGES_FOLDER "${PROJECT_SOURCE_DIR}/EXR_Images" CACHE PATH "EXR_IMAGES_FOLDER")
SET(GLSL_FOLDER "${PROJECT_SOURCE_DIR}/glsl" CACHE PATH "GLSL_FOLDER")
SET(MODELS_FOLDER "${PROJECT_SOURCE_DIR}/models" CACHE PATH "MODELS_FOLDER")

option(BUILD_EXAMPLES "Build Examples" ON)
option(BUILD_OPENGL_HELPERS "Build Classes to handle some opengl features" ON)
option(BUILD_UNIT_TEST "Build tests" ON)
option(BUILD_GLEW "Build glew" ON)
option(BUILD_GLFW "Build glfw" ON)
option(BUILD_TOOLS "Build tools" ON)

#SET(OPENEXRLIBS "Half Iex IlmImf IlmThread Imath")

include_directories("${PROJECT_SOURCE_DIR}/ext/")
include_directories("${PROJECT_SOURCE_DIR}/ext/im/include")
include_directories("${PROJECT_SOURCE_DIR}/ext/eigen")
include_directories("${PROJECT_SOURCE_DIR}/ext/glfw/include")
include_directories("${PROJECT_SOURCE_DIR}/ext/glfw-3.0/include")
include_directories("${PROJECT_SOURCE_DIR}/ext/openEXR/include/")
include_directories(${PROJECT_SOURCE_DIR} )

link_directories("${PROJECT_SOURCE_DIR}/ext/openEXR/msvc10/" )
link_directories("${PROJECT_SOURCE_DIR}/ext/lib/msvc10/" )

add_definitions(-DGLSL_DIR="${GLSL_FOLDER}")
add_definitions(-DMODELS_DIR="${MODELS_FOLDER}")
add_definitions(-DEXR_IMAGES_DIR="${EXR_IMAGES_FOLDER}")

add_definitions( -DGLEW_STATIC)
add_definitions( -D_USE_MATH_DEFINES)

#add_definitions(-DNCHOLMOD)

FIND_PACKAGE(Boost REQUIRED)
IF (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIR})
	link_directories(${Boost_LIBRARY_DIRS})
	add_definitions( "-DHAS_BOOST" )
ENDIF()

FIND_PACKAGE(OpenMP)
if(OPENMP_FOUND)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_subdirectory(glsl)


# add_subdirectory(ext/eigen)
 add_subdirectory(ext/im)

add_subdirectory(Base)
add_subdirectory(Engine)
add_subdirectory(rEngine)
add_subdirectory(FluidSimulator)
add_subdirectory(Image)
add_subdirectory(OpenEXRHelpers)
add_subdirectory(Geometry)
add_subdirectory(Math)
add_subdirectory(Volume)
add_subdirectory(Fractals)
add_subdirectory(IMHelpers)
add_subdirectory(SuperEngine)
add_subdirectory(Util)
add_subdirectory(Physics)
add_subdirectory(RBF)

add_subdirectory(ext/tinyxml2)

if(BUILD_GLEW)
	add_subdirectory(ext/glew)
endif()

if(BUILD_GLFW)
	add_subdirectory(ext/glfw)
	add_subdirectory(ext/glfw-3.0)
endif()


if(BUILD_TOOLS)
	add_subdirectory(Tools)
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