cmake_minimum_required (VERSION 2.8.4) 


file(GLOB IM_H
    "include/*.h"
)

file(GLOB IM_SRC
    "src/*.c"
	"src/*.cpp"
	"src/*.hpp"
	"src/*.h"
    "src/process/*.c"
	"src/process/*.hpp"
	"src/process/*.cpp"
	"src/process/*.h"
)

file(GLOB IM_FFTW_SRC
    "src/fftw/*.c"
	"src/fftw/*.hpp"
	"src/fftw/*.cpp"
	"src/fftw/*.h"
)
file(GLOB IM_exif_SRC
    "src/libexif/*.c"
	"src/libexif/*.hpp"
	"src/libexif/*.cpp"
	"src/libexif/*.h"
)
file(GLOB IM_jasper_SRC
    "src/libjasper/*/*.c"
	"src/libjasper/*/*.hpp"
	"src/libjasper/*/*.cpp"
	"src/libjasper/*/*.h"
)
file(GLOB IM_jpeg_SRC
    "src/libjpeg/*.c"
	"src/libjpeg/*.hpp"
	"src/libjpeg/*.cpp"
	"src/libjpeg/*.h"
)
file(GLOB IM_lzf_SRC
    "src/liblzf/*.c"
	"src/liblzf/*.hpp"
	"src/liblzf/*.cpp"
	"src/liblzf/*.h"
)
file(GLOB IM_png_SRC
    "src/libpng/*.c"
	"src/libpng/*.hpp"
	"src/libpng/*.cpp"
	"src/libpng/*.h"
)
file(GLOB IM_libtiff_SRC
    "src/libtiff/*.c"
	"src/libtiff/*.hpp"
	"src/libtiff/*.cpp"
	"src/libtiff/*.h"
)
file(GLOB IM_zlib_SRC
    "src/zlib/*.c"
	"src/zlib/*.hpp"
	"src/zlib/*.cpp"
	"src/zlib/*.h"
)

add_definitions( -DJAS_GEO_OMIT_PRINTING_CODE)
add_definitions( -DIM_PROCESS)


include_directories("${PROJECT_SOURCE_DIR}/ext/im/src/libjasper/")
include_directories("${PROJECT_SOURCE_DIR}/ext/im/src/libexif/")
include_directories("${PROJECT_SOURCE_DIR}/ext/im/src/libjpeg/")
include_directories("${PROJECT_SOURCE_DIR}/ext/im/src/liblzf/")
include_directories("${PROJECT_SOURCE_DIR}/ext/im/src/libtiff/")
include_directories("${PROJECT_SOURCE_DIR}/ext/im/src/libpng/")
include_directories("${PROJECT_SOURCE_DIR}/ext/im/src/fftw/")
include_directories("${PROJECT_SOURCE_DIR}/ext/im/src/zlib/")
include_directories("${PROJECT_SOURCE_DIR}/ext/im/src/")

add_library(fftw ${IM_FFTW_SRC})
add_library(libexif ${IM_exif_SRC})
add_library(libjasper ${IM_jasper_SRC})
add_library(libjpeg ${IM_jpeg_SRC})
add_library(liblzf ${IM_lzf_SRC})
add_library(libpng ${IM_png_SRC})
add_library(libtiff ${IM_libtiff_SRC})
add_library(zlib ${IM_zlib_SRC})


add_library(IM ${IM_SRC} ${IM_H})


set_target_properties(IM 
  PROPERTIES
  FOLDER "ext/IM")
set_target_properties(fftw 
  PROPERTIES
  FOLDER "ext/IM")
set_target_properties(libexif 
  PROPERTIES
  FOLDER "ext/IM")
set_target_properties(libjasper 
  PROPERTIES
  FOLDER "ext/IM")
set_target_properties(libjpeg 
  PROPERTIES
  FOLDER "ext/IM")
set_target_properties(liblzf 
  PROPERTIES
  FOLDER "ext/IM")
set_target_properties(libpng 
  PROPERTIES
  FOLDER "ext/IM")
set_target_properties(libtiff 
  PROPERTIES
  FOLDER "ext/IM")
set_target_properties(zlib 
  PROPERTIES
  FOLDER "ext/IM")
  
  

target_link_libraries(IM fftw libexif libjasper libjpeg liblzf libpng libtiff zlib)




