#//-----------------------------------------------------------------------------
#//	LAUNCHER_MAIN.VPC
#//
#//	Project Script
#//-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.16)
#dont build this on dedicated build
project(launcher_main)
set(SRCDIR "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH ${SRCDIR}/cmake)
set(OUTBINDIR "${SRCDIR}/../game")

include(${CMAKE_MODULE_PATH}/detect_platform.cmake)

if( WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(OUTBINNAME "csgo")
elseif( WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OUTBINNAME "csgo_win64")
elseif( OSX32 )
    set(OUTBINNAME "csgo_osx")
elseif( OSX64 )
    set(OUTBINNAME "csgo_osx64")
elseif( LINUX32 )
    set(OUTBINNAME "csgo_linux")
elseif( LINUX64 )
    set(OUTBINNAME "csgo_linux64")
endif()

set(NOSTINKYLINKIES "1")
set(NOSKELETONBASE "1")

include(${CMAKE_MODULE_PATH}/source_exe_base.cmake)

target_sources(${OUTBINNAME} PRIVATE main.cpp)

# I dont know if mingw can compile resource files
if( MSVC )
	target_sources(${OUTBINNAME} PRIVATE launcher_main.rc)
endif()

if( LINUXALL )
    target_link_libraries(${OUTBINNAME} dl pthread)
endif()
