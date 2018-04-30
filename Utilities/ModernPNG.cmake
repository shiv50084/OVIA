cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project(ModernPNG)

add_definitions(-D_LARGEFILE_SOURCE)
add_definitions(-D_LARGEFILE64_SOURCE)
add_definitions(-D_FILE_OFFSET_BITS=64)

set(ModernPNG_BUILD "${CMAKE_CURRENT_SOURCE_DIR}/../BUILD/${project}/${ARCHITECTURE}/${BUILDTYPE}")

add_executable(${project} ${CMAKE_CURRENT_SOURCE_DIR}/ModernPNG.c)
TARGET_LINK_LIBRARIES(${project} ${libModernPNG} ${libPCM} ${libFoundationIO} m)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)
	set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}" -flto)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" -std=c11 -ferror-limit=1024 -Weverything -fcolor-diagnostics -fconst-strings -fshort-enums -vectorize-loops -funroll-loops -funsigned-char -Wno-c99-compat -fstrict-enums)

	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O1")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")
endif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG
