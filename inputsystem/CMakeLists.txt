#//-----------------------------------------------------------------------------
#//	INPUTSYSTEM.VPC
#//
#//	Project Script
#//-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.16)

project(inputsystem_client)
set(SRCDIR "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH ${SRCDIR}/cmake)
set(OUTBINNAME "inputsystem_client")
set(OUTBINDIR ${SRCDIR}/../game/bin)

include(${CMAKE_MODULE_PATH}/detect_platform.cmake)
include(${CMAKE_MODULE_PATH}/source_dll_base.cmake)

add_definitions(-DNO_STRING_T -DVERSION_SAFE_STEAM_API_INTERFACES -DPROTECTED_THINGS_ENABLE)

if( LINUXALL )
    target_link_libraries(${OUTBINNAME} SDL2)
endif()

target_sources(${OUTBINNAME} PRIVATE "inputstacksystem.cpp")
target_sources(${OUTBINNAME} PRIVATE "inputsystem.cpp")
target_sources(${OUTBINNAME} PRIVATE "inputsystem.h")
if( WIN32 )
    target_sources(${OUTBINNAME} PRIVATE "joystick.cpp")
elseif( OSXALL )
    target_sources(${OUTBINNAME} PRIVATE "joystick_osx.cpp")
elseif( LINUXALL )
    target_sources(${OUTBINNAME} PRIVATE "joystick_linux.cpp")
endif()
target_sources(${OUTBINNAME} PRIVATE "steamcontroller.cpp")
target_sources(${OUTBINNAME} PRIVATE "key_translation.cpp")
target_sources(${OUTBINNAME} PRIVATE "key_translation.h")
if( WIN32 )
	target_sources(${OUTBINNAME} PRIVATE "xcontroller.cpp")
    #$File	"xcontroller.cpp"		[!$POSIX]
endif()
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/common/platforminputdevice.cpp")

target_link_libraries(${OUTBINNAME} tier2_client mathlib_client vstdlib_client)
#Requires evil proprietary link to libsteam_api
if( MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4 )
	target_link_libraries(${OUTBINNAME} ${LIBPUBLIC}/steam_api.lib Winmm.lib)
elseif( MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 8 )
	target_link_libraries(${OUTBINNAME} ${LIBPUBLIC}/steam_api64.lib Winmm.lib)
else()
	target_link_libraries(${OUTBINNAME} ${LIBPUBLIC}/libsteam_api.so)
endif()
