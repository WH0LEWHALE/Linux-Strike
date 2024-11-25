# Cmake script for hk_base

cmake_minimum_required(VERSION 3.16)
project(hk_base)
set(SRCDIR "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH ${SRCDIR}/cmake)
set(OUTLIBNAME "hk_base")

include(${CMAKE_MODULE_PATH}/detect_platform.cmake)
include(${CMAKE_MODULE_PATH}/source_lib_base.cmake)

include_directories(${SRCDIR}/ivp/havana/havok)

add_definitions(-D_LIB -DVPHYSICS_EXPORTS)

if( LINUXALL )
    target_compile_options(${OUTLIBNAME} PRIVATE -fpic -fno-semantic-interposition)
endif()

if(MSVC)
	add_definitions(-D_ALLOW_KEYWORD_MACROS)
endif()

target_sources(${OUTLIBNAME} PRIVATE array/array.cpp)
target_sources(${OUTLIBNAME} PRIVATE hash/hash.cpp)
target_sources(${OUTLIBNAME} PRIVATE memory/memory.cpp)
target_sources(${OUTLIBNAME} PRIVATE memory/memory_util.cpp)
target_sources(${OUTLIBNAME} PRIVATE stopwatch/stopwatch.cpp)
target_sources(${OUTLIBNAME} PRIVATE base_types.cpp)
target_sources(${OUTLIBNAME} PRIVATE console.cpp)
