cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project(libModernPNG)

set(RootDirectory "${CMAKE_CURRENT_SOURCE_DIR}/..")

add_definitions(-D_LARGEFILE_SOURCE)
add_definitions(-D_LARGEFILE64_SOURCE)
add_definitions(-D_FILE_OFFSET_BITS=64)

set(libModernPNG_BUILD "${CMAKE_CURRENT_SOURCE_DIR}/../BUILD/${project}/${ARCHITECTURE}/${BUILDTYPE}")
set(ARCHIVE_OUTPUT_DIRECTORY "${libModernPNG_BUILD}")

set(libModernPNG_Dir "${CMAKE_CURRENT_SOURCE_DIR}")

set(libModernPNG_Headers 
	"${libModernPNG_Dir}/include/libModernPNG.h"
	"${libModernPNG_Dir}/include/Private/libModernPNG_Types.h"
	"${libModernPNG_Dir}/include/Private/libModernPNG_Tables.h"
	"${libModernPNG_Dir}/include/Private/Common/libModernPNG_EntropyCoders.h"
	"${libModernPNG_Dir}/include/Private/Decode/libModernPNG_Decode.h"
	"${libModernPNG_Dir}/include/Private/Decode/libModernPNG_ReadChunks.h"
	"${libModernPNG_Dir}/include/Private/Encode/libModernPNG_Encode.h"
)

set(libModernPNG_Sources
	"${libModernPNG_Dir}/src/libModernPNG.c"
	"${libModernPNG_Dir}/src/Common/libModernPNG_AddMetadata.c"
	"${libModernPNG_Dir}/src/Common/libModernPNG_EntropyCoders.c"
	"${libModernPNG_Dir}/src/Common/libModernPNG_ExtractMetadata.c"
	"${libModernPNG_Dir}/src/Decode/libModernPNG_Decode.c"
	"${libModernPNG_Dir}/src/Decode/libModernPNG_ReadChunks.c"
	"${libModernPNG_Dir}/src/Encode/libModernPNG_ComposeChunks.c"
	"${libModernPNG_Dir}/src/Encode/libModernPNG_Encode.c"
	"${libModernPNG_Dir}/src/Encode/libModernPNG_WriteChunks.c"
)

add_library(${project} [static|shared] ${libModernPNG_Sources} ${libModernPNG_Headers})

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)
	set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}" -flto)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" -std=c11 -ferror-limit=1024 -Weverything -fcolor-diagnostics -fconst-strings -fshort-enums -vectorize-loops -funroll-loops -funsigned-char -Wno-c99-compat -fstrict-enums)

	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O1")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")
endif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)
