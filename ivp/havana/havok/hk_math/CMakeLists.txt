# Cmake script for hk_math

cmake_minimum_required(VERSION 3.16)
project(hk_math)
set(SRCDIR "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH ${SRCDIR}/cmake)
set(OUTLIBNAME "hk_math")

include(${CMAKE_MODULE_PATH}/detect_platform.cmake)
include(${CMAKE_MODULE_PATH}/source_lib_base.cmake)

include_directories(${SRCDIR}/ivp/ivp_utility)
include_directories(${SRCDIR}/ivp/havana/havok)

add_definitions(-D_LIB)

if( LINUXALL )
    target_compile_options(${OUTLIBNAME} PRIVATE -fpic -fno-semantic-interposition -fno-strict-aliasing)
endif()

target_sources(${OUTLIBNAME} PRIVATE quaternion/quaternion.cpp)
target_sources(${OUTLIBNAME} PRIVATE quaternion/quaternion_util.cpp)
target_sources(${OUTLIBNAME} PRIVATE vector3/vector3.cpp)
target_sources(${OUTLIBNAME} PRIVATE vector3/vector3_util.cpp)
target_sources(${OUTLIBNAME} PRIVATE dense_vector.cpp)
target_sources(${OUTLIBNAME} PRIVATE densematrix.cpp)
target_sources(${OUTLIBNAME} PRIVATE densematrix_util.cpp)
target_sources(${OUTLIBNAME} PRIVATE eulerangles.cpp)
target_sources(${OUTLIBNAME} PRIVATE math.cpp)
target_sources(${OUTLIBNAME} PRIVATE matrix3.cpp)
target_sources(${OUTLIBNAME} PRIVATE rotation.cpp)
target_sources(${OUTLIBNAME} PRIVATE transform.cpp)
target_sources(${OUTLIBNAME} PRIVATE vecmath.cpp)
