#//-----------------------------------------------------------------------------
#//	SOUNDSYSTEM_LOWLEVEL.VPC
#//
#//	Project Script
#//-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.16)
project(soundsystem_lowlevel)
set(SRCDIR "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH ${SRCDIR}/cmake)
set(OUTLIBNAME "soundsystem_lowlevel")

include(${CMAKE_MODULE_PATH}/detect_platform.cmake)
include(${CMAKE_MODULE_PATH}/source_lib_base.cmake)

#windows
#$Include "$SRCDIR\vpc_scripts\dxsdk_macros.vpc"
#$Configuration
#{
#	$Compiler
#	{
#		$AdditionalIncludeDirectories	"$BASE;$SRCDIR\thirdparty\dxsdk\include;$DXSDKINCLUDE"					[$WINDOWS]
#	}
#}

if( MSVC AND WIN32 )
	include_directories(${SRCDIR}/thirdparty/dxsdk/include)
	#if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	#	target_link_libraries(${OUTLIBNAME} ${SRCDIR}/thirdparty/dxsdk/lib/x64/dsound.lib ${SRCDIR}/thirdparty/dxsdk/lib/x64/dxguid.lib)
	#else()
	#	target_link_libraries(${OUTLIBNAME} ${SRCDIR}/thirdparty/dxsdk/lib/x86/dsound.lib ${SRCDIR}/thirdparty/dxsdk/lib/x86/dxguid.lib)
	#endif()
endif()

if( WIN32 )
    target_sources(${OUTLIBNAME} PRIVATE "device_dsound.cpp")
	target_sources(${OUTLIBNAME} PRIVATE "device_xaudio2.cpp")
	target_sources(${OUTLIBNAME} PRIVATE "windows_audio.cpp")
endif()
if( LINUXALL )
    target_compile_options(${OUTLIBNAME} PRIVATE -fpic -fno-semantic-interposition)
endif()

if( POSIX )
    target_sources(${OUTLIBNAME} PRIVATE "device_sdl.cpp")
endif()
target_sources(${OUTLIBNAME} PRIVATE "device_null.cpp")
target_sources(${OUTLIBNAME} PRIVATE "mix.cpp")
target_sources(${OUTLIBNAME} PRIVATE "simple_filter.cpp")
