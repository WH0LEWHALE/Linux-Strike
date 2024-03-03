#//-----------------------------------------------------------------------------
#//	VAUDIO_MINIMP3.VPC
#//
#//	Project Script
#//-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.16)
project(vaudio_minimp3)
set(SRCDIR "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH ${SRCDIR}/cmake)
set(OUTBINNAME "vaudio_minimp3_client")
set(OUTBINDIR ${SRCDIR}/../game/bin)

set(NOSTINKYLINKIES "1") #link this project carefully ourselves

include(${CMAKE_MODULE_PATH}/detect_platform.cmake)
include(${CMAKE_MODULE_PATH}/source_dll_base.cmake)

target_sources(${OUTBINNAME} PRIVATE "vaudio_minimp3.cpp")

target_link_libraries(${OUTBINNAME} libtier0_client tier1_client )