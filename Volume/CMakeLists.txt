 cmake_minimum_required (VERSION 2.6) 


set(Volume_SRC LevelSet.cpp ScalarField.cpp VectorField.cpp)
set(Volume_H   LevelSet.h ScalarField.h VectorField.h Field3D.h)
set(Volume_HPP   LevelSet.hpp ScalarField.hpp VectorField.hpp Field3D.hpp)

add_library(Volume ${Volume_SRC} ${Volume_H} ${Volume_HPP})

target_link_libraries(Volume BoundingGeometry)

set_target_properties(Volume
  PROPERTIES
  FOLDER "Libraries")