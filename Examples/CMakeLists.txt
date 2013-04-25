cmake_minimum_required (VERSION 2.6) 


FIND_PACKAGE(OpenGL REQUIRED)

INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})


#add_subdirectory(FluidRenderer)
add_subdirectory(LevelSet)
add_subdirectory(Mesh)
add_subdirectory(GameOfLife)
add_subdirectory(Shaders)
add_subdirectory(UltrasoundSurfaceExtaction)
add_subdirectory(UltrasoundSurfacePoints)
add_subdirectory(OpenEXR)
add_subdirectory(GMRES)
add_subdirectory(FractalsExamples)
add_subdirectory(XMLExample)
add_subdirectory(Cloth)
add_subdirectory(KDTree)
add_subdirectory(RBFExample)
add_subdirectory(RBFSurfaceExtraction)
add_subdirectory(RBFFastEvalExample)
add_subdirectory(Test)
add_subdirectory(EngineExample)
add_subdirectory(LeastSquarePlane)
add_subdirectory(IMExample)