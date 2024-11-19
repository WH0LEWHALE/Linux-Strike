#CMake for all matchmaking targets.
cmake_minimum_required(VERSION 3.16)
project(matchmaking_libraries)
set(SRCDIR "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH ${SRCDIR}/cmake)
include(${CMAKE_MODULE_PATH}/detect_platform.cmake)

if( NOT DEDICATED )
#Normal MatchmakingBase
set(OUTLIBNAME "matchmakingbase")
set(MM_DS "0")

include( ./matchmaking_base_inc.cmake )
target_compile_definitions(${OUTLIBNAME} PRIVATE -DMATCHMAKING_DLL)
else() # DEDICATED
#Dedicated Server MatchmakingBase
set(OUTLIBNAME "matchmakingbase_ds")
set(MM_DS "1")

include( ./matchmaking_base_inc.cmake )
target_compile_definitions(${OUTLIBNAME} PRIVATE -DMATCHMAKING_DS_DLL -DSWDS)
endif() # ENDIF DEDICATED

#CSGO Matchmaking
if( NOT DEDICATED )
set(OUTBINNAME "matchmaking_client")

include( ./matchmaking_cstrike15_inc.cmake )
target_compile_definitions(${OUTBINNAME} PRIVATE -DMATCHMAKING_DLL)
target_link_libraries(${OUTBINNAME} matchmakingbase)
target_link_libraries(${OUTBINNAME} tier2_client mathlib_client)
target_link_libraries(${OUTBINNAME} kisak_gcsdk_client)
#target_link_libraries(${OUTBINNAME} ${LIBPUBLIC}/libsteam_api.so) # link to proprietary steamapi
if( (NOT DEFINED NO_STEAM) )
    #Looks like we have to include libsteam_api
	if( MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4 )
		target_link_libraries(${OUTBINNAME} ${LIBPUBLIC}/steam_api.lib)
	elseif( MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 8 )
		target_link_libraries(${OUTBINNAME} ${LIBPUBLIC}/steam_api64.lib)
	else()
		target_link_libraries(${OUTBINNAME} ${LIBPUBLIC}/libsteam_api.so)
	endif()
endif()
else() # DEDICATED
#CSGO Matchmaking DS
set(OUTBINNAME "matchmaking_ds_client")

include( ./matchmaking_cstrike15_inc.cmake )
target_compile_definitions(${OUTBINNAME} PRIVATE -DMATCHMAKING_DS_DLL -DSWDS)
target_link_libraries(${OUTBINNAME} matchmakingbase_ds)
target_link_libraries(${OUTBINNAME} tier2_client mathlib_client)
target_link_libraries(${OUTBINNAME} kisak_gcsdk_client)
#target_link_libraries(${OUTBINNAME} ${LIBPUBLIC}/libsteam_api.so) # link to proprietary steamapi

if( (NOT DEFINED NO_STEAM) )
    #Looks like we have to include libsteam_api
	if( MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4 )
		target_link_libraries(${OUTBINNAME} ${LIBPUBLIC}/steam_api.lib)
	elseif( MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 8 )
		target_link_libraries(${OUTBINNAME} ${LIBPUBLIC}/steam_api64.lib)
	else()
		target_link_libraries(${OUTBINNAME} ${LIBPUBLIC}/libsteam_api.so)
	endif()
endif()
endif()