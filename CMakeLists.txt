# Kisak-Strike: Gentoo Offensive
# Main CMake File

# List of targets from VPC
#                        Dmserializers*         Matchmaking_DS_CSGO*   ServerBrowser*         tier3*
#                        Dmxloader*             materialsystem*        Server_CSGO*           Unitlib*
# appframework*          engine*                mathlib*               shaderapiempty*        valve_avi*
# bitmap*                engine_ds              mathlib_extended*      shaderlib*             vgui2*
# bitmap_byteswap*       Fgdlib*                matsys_controls*                              vgui_controls*
# bonesetup*             filesystem_stdio*      meshutils*                                    vguimatsurface*
# Bzip2*                 inputsystem*           particles*             soundemittersystem*    vgui_surfacelib*
# choreoobjects*         interfaces*            quickhull*             soundsystem_lowlevel*  videocfg*
#                        launcher*              Raytrace*              stdshader_dx9*         vpklib*
#                        launcher_main*                                studiorender*          vscript*
#                        localize*                                                            vstdlib*
# Client_CSGO*           matchmakingbase*       resourcefile*          tier0*                 vtf*
# datacache*             matchmakingbase_ds*    responserules_runtime* tier1*
# Dedicated*             Matchmaking_CSGO*      SceneFileCache*        tier2*

cmake_minimum_required(VERSION 3.16)
project(KisakStrike)

include(kisak-strike-build-options.cmake)

add_subdirectory(appframework)
add_subdirectory(external/crypto++-5.61)
#add_subdirectory(external/freetype-2.10.4)
add_subdirectory(interfaces)
#add_subdirectory(fgdlib)
add_subdirectory(gcsdk)
add_subdirectory(tier0)
add_subdirectory(tier1)
add_subdirectory(tier2)
add_subdirectory(tier3)
add_subdirectory(vstdlib)
add_subdirectory(mathlib) #also does mathlib_extended.
add_subdirectory(vpklib)
if(NOT DEDICATED)
add_subdirectory(vgui2/matsys_controls)
add_subdirectory(vgui2/src)
add_subdirectory(vgui2/vgui_controls)
add_subdirectory(vgui2/vgui_surfacelib)
endif()
if(WIN32)
	add_subdirectory(soundsystem)
	message("You're using an operating system that we do not officially support, you may encounter errors during compilation.")
endif()
if(APPLE)
	message("You're using an operating system that we do not officially support, you may encounter errors during compilation.")
endif()
if(NOT DEDICATED)
add_subdirectory(soundsystem/lowlevel)
endif()
add_subdirectory(thirdparty/quickhull)
add_subdirectory(thirdparty/zlib-1.2.8)
add_subdirectory(thirdparty/libpng-1.5.30)
add_subdirectory(thirdparty/protobuf-3.5.1/cmake)
add_subdirectory(utils/bzip2)
add_subdirectory(utils/jpeglib)
add_subdirectory(unitlib)
if( NOT DEDICATED )
add_subdirectory(videocfg)
endif()
add_subdirectory(vtf)
if( NOT DEDICATED )
add_subdirectory(avi)
endif()
add_subdirectory(bitmap) #also does bitmap_byteswap
add_subdirectory(bonesetup)
add_subdirectory(choreoobjects)
add_subdirectory(datacache)
add_subdirectory(dmserializers)
add_subdirectory(dmxloader)
if( NOT DEDICATED )
	add_subdirectory(engine/voice_codecs/minimp3)
	add_subdirectory(engine)
else()
	add_subdirectory(engine_ds)
endif()
add_subdirectory(filesystem)
if( NOT DEDICATED )
add_subdirectory(inputsystem)
endif()
add_subdirectory(localize)
add_subdirectory(materialsystem)
add_subdirectory(materialsystem/shaderlib)
add_subdirectory(materialsystem/stdshaders)
if( DEDICATED )
add_subdirectory(materialsystem/shaderapiempty)
else()
add_subdirectory(materialsystem/shaderapidx9)
endif()
add_subdirectory(meshutils)
add_subdirectory(particles)
add_subdirectory(raytrace)
add_subdirectory(resourcefile)
add_subdirectory(responserules/runtime)
add_subdirectory(scenefilecache)
if( NOT DEDICATED )
add_subdirectory(serverbrowser)
endif()
add_subdirectory(soundemittersystem)
add_subdirectory(studiorender)
if(NOT WIN32 AND NOT GL AND NOT DEDICATED)
	add_subdirectory(togl)
endif()
if(NOT DEDICATED)
add_subdirectory(vguimatsurface)
endif()
add_subdirectory(vscript)
add_subdirectory(game/client) #CSGO Client
add_subdirectory(game/server) #CSGO Server
add_subdirectory(matchmaking) #Does all 4 matchmaking projects

if( USE_IPHYS_PHYSICS )
    add_definitions(-DUSE_IPHYS_PHYSICS)
    message("Using Open-Source Linux-Strike Physics rebuild!\n")
    add_subdirectory(ivp/havana/havok/hk_base)
    add_subdirectory(ivp/havana/havok/hk_math)
    add_subdirectory(ivp/havana/havok/hk_physics/constraint)
    add_subdirectory(ivp/ivp_compact_builder)
    add_subdirectory(ivp/ivp_physics)
    add_subdirectory(vphysics)
#Experimental open source Bullet Physics
elseif( USE_BULLET_PHYSICS )
    add_definitions(-DUSE_BULLET_PHYSICS)
    #lwss: threading is experimental
    if( USE_BULLET_PHYSICS_THREADED )
        add_definitions(-DBT_THREADSAFE -DBT_USE_OPENMP)
        set(BULLET2_MULTITHREADING ON CACHE BOOL "" FORCE)
        set(BULLET2_USE_OPEN_MP_MULTITHREADING ON CACHE BOOL "" FORCE)
    endif()
    message("Using Bullet Open-Source Physics\n")
    set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
    set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
    set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
    set(BUILD_BULLET3 OFF CACHE BOOL "" FORCE)
    set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
    set(BUILD_ENET OFF CACHE BOOL "" FORCE)
    set(BUILD_CLSOCKET OFF CACHE BOOL "" FORCE)
    set(OpenGL_GL_PREFERENCE LEGACY)
    if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
        message("Adding Bullet debug flag\n")
        add_definitions(-DBT_DEBUG)
        add_definitions(-DBT_DEBUG_MEMORY_ALLOCATIONS)
    endif()
    add_subdirectory(thirdparty/bullet3)
    add_subdirectory(vphysics_bullet)
else()
message("How dare you,\nUsing Closed-Source Physics made by Valve")
endif()

if( DEDICATED )
	add_subdirectory(dedicated)
	add_subdirectory(dedicated_main)
endif()
if( NOT DEDICATED )
	add_subdirectory(launcher)
	add_subdirectory(launcher_main) #The actual executable that just dlopen's launcher
endif()

if( NOT DEDICATED )
	if( USE_SCALEFORM )
    		add_subdirectory(scaleformui) #DUMMY cmake project so CLion can do analysis
	elseif( USE_ROCKETUI ) #Custom Kisak-Strike UI powered by RmlUI
    		add_subdirectory(thirdparty/RmlUi)
    		add_subdirectory(rocketui)
	endif()
endif()

message("Made with ❤️ by yungDoom")
