cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

set(RootDirectory "${CMAKE_CURRENT_SOURCE_DIR}/..")

include("${RootDirectory}/Dependencies/FoundationIO/Projects/CMakeLists.txt" [FoundationIOFound])

project(libPCM)

set(CMAKE_INSTALL_PREFIX "${/usr/local/Packages/${project}}")

add_definitions(-D_LARGEFILE_SOURCE)
add_definitions(-D_LARGEFILE64_SOURCE)
add_definitions(-D_FILE_OFFSET_BITS=64)



set(BUILD "${RootDirectory}/BUILD/${ARCHITECTURE}/${BUILDTYPE}")
set(ARCHIVE_OUTPUT_DIRECTORY "${BUILD}")

set(libPCM_Dir "${RootDirectory}/libPCM")

set(libPCM_Include "${libPCM_Dir}/include")

set(libPCM_Headers 
	"${libPCM_Include}/libPCM.h"
	"${libPCM_Include}/Private/libPCMTypes.h"
	"${libPCM_Include}/Private/Audio/AIFCommon.h"
	"${libPCM_Include}/Private/Audio/W64Common.h"
	"${libPCM_Include}/Private/Audio/WAVCommon.h"
	"${libPCM_Include}/Private/Image/BMPCommon.h"
	"${libPCM_Include}/Private/Image/PXMCommon.h"
)

set(libPCM_Source_Dir "${libPCM_Dir}/src")

set(libPCM_Sources
	"${libPCM_Source_Dir}/libPCM.c"
	"${libPCM_Source_Dir}/Audio/AIFDecoder.c"
	"${libPCM_Source_Dir}/Audio/AIFEncoder.c"
	"${libPCM_Source_Dir}/Audio/W64Decoder.c"
	"${libPCM_Source_Dir}/Audio/W64Encoder.c"
	"${libPCM_Source_Dir}/Audio/WAVDecoder.c"
	"${libPCM_Source_Dir}/Audio/WAVEncoder.c"
	"${libPCM_Source_Dir}/Image/BMPDecoder.c"
	"${libPCM_Source_Dir}/Image/BMPEncoder.c"
	"${libPCM_Source_Dir}/Image/PXMDecoder.c"
	"${libPCM_Source_Dir}/Image/PXMEncoder.c"
)
add_library(libPCM ${libPCM_Sources})

set(TrimSilence "${RootDirectory}")

set(TrimSilence "${RootDirectory}/TrimSilence/TrimSilence.c")

add_executable(TrimSilence ${TrimSilence})
TARGET_LINK_LIBRARIES(TrimSilence ${libPCM} ${FoundationIO} ${math})
# [Debug|Release]

include_directories(${libPCM_Include})

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)
	set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}" -flto)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" -std=c11 -ferror-limit=1024 -Weverything -fcolor-diagnostics -fconst-strings -fshort-enums -vectorize-loops -funroll-loops -funsigned-char -Wno-c99-compat -fstrict-enums)

	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O1")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")
endif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)

find_program(iwyu_path NAMES include-what-you-use iwyu)
if(NOT iwyu_path)
message(FATAL_ERROR "Could not find the program include-what-you-use")
endif()

set_property(TARGET TrimSilence PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
