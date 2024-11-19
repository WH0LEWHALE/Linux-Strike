#//-----------------------------------------------------------------------------
#//	SOUNDSYSTEM.VPC
#//
#//	Project Script
#//-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.16)
project(soundsystem_client)
set(SRCDIR "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH ${SRCDIR}/cmake)
set(OUTBINNAME "soundsystem_client")
set(OUTBINDIR ${SRCDIR}/../game/bin)

include(${CMAKE_MODULE_PATH}/detect_platform.cmake)
include(${CMAKE_MODULE_PATH}/source_dll_base.cmake)


if( MSVC AND WIN32 )
	include_directories(${SRCDIR}/thirdparty/dxsdk/include)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		target_link_libraries(${OUTBINNAME} ${SRCDIR}/thirdparty/dxsdk/lib/x64/dsound.lib ${SRCDIR}/thirdparty/dxsdk/lib/x64/dxguid.lib)
	else()
		target_link_libraries(${OUTBINNAME} ${SRCDIR}/thirdparty/dxsdk/lib/x86/dsound.lib ${SRCDIR}/thirdparty/dxsdk/lib/x86/dxguid.lib)
	endif()
	target_link_libraries(${OUTBINNAME} winmm.lib)
endif()

add_definitions(-DSOUNDSYSTEM_EXPORTS)

target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/public/sentence.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/public/phonemeconverter.cpp")
target_sources(${OUTBINNAME} PRIVATE "snd_audio_source.cpp")
target_sources(${OUTBINNAME} PRIVATE "snd_dev_wave.cpp")
target_sources(${OUTBINNAME} PRIVATE "snd_io.cpp")
target_sources(${OUTBINNAME} PRIVATE "snd_wave_mixer.cpp")
target_sources(${OUTBINNAME} PRIVATE "snd_wave_mixer_adpcm.cpp")
target_sources(${OUTBINNAME} PRIVATE "snd_wave_source.cpp")
target_sources(${OUTBINNAME} PRIVATE "soundsystem.cpp")

target_link_libraries(${OUTBINNAME} soundsystem_lowlevel vstdlib_client mathlib_client tier2_client)