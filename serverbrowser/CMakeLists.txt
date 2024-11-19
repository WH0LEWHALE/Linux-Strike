#//-----------------------------------------------------------------------------
#//	SERVERBROWSER.VPC
#//
#//	Project Script
#//-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)
project(serverbrowser_client)
set(SRCDIR "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH ${SRCDIR}/cmake)
set(OUTBINNAME "serverbrowser_client")
set(OUTBINDIR ${SRCDIR}/../game/bin)

include(${CMAKE_MODULE_PATH}/detect_platform.cmake)
include(${CMAKE_MODULE_PATH}/source_dll_base.cmake)

add_definitions(-DVERSION_SAFE_STEAM_API_INTERFACES -DSERVERBROWSER_EXPORTS -DGAME_SRC)
#if( WIN32 AND NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
#   add_definitions(-D_USE_32BIT_TIME_T)
#endif()

target_sources(${OUTBINNAME} PRIVATE "BaseGamesPage.cpp")
target_sources(${OUTBINNAME} PRIVATE "blacklistedservers.cpp")
target_sources(${OUTBINNAME} PRIVATE "CustomGames.cpp")
target_sources(${OUTBINNAME} PRIVATE "DialogAddServer.cpp")
target_sources(${OUTBINNAME} PRIVATE "DialogGameInfo.cpp")
target_sources(${OUTBINNAME} PRIVATE "DialogServerPassword.cpp")
target_sources(${OUTBINNAME} PRIVATE "FavoriteGames.cpp")
target_sources(${OUTBINNAME} PRIVATE "FriendsGames.cpp")
target_sources(${OUTBINNAME} PRIVATE "HistoryGames.cpp")
target_sources(${OUTBINNAME} PRIVATE "InternetGames.cpp")
target_sources(${OUTBINNAME} PRIVATE "LanGames.cpp")
target_sources(${OUTBINNAME} PRIVATE "ModList.cpp")
target_sources(${OUTBINNAME} PRIVATE "ServerBrowser.cpp")
target_sources(${OUTBINNAME} PRIVATE "ServerBrowserDialog.cpp")
target_sources(${OUTBINNAME} PRIVATE "ServerContextMenu.cpp")
target_sources(${OUTBINNAME} PRIVATE "ServerListCompare.cpp")
target_sources(${OUTBINNAME} PRIVATE "SpectateGames.cpp")
target_sources(${OUTBINNAME} PRIVATE "VACBannedConnRefusedDialog.cpp")
target_sources(${OUTBINNAME} PRIVATE "VACBannedConnRefusedDialog.h")
target_sources(${OUTBINNAME} PRIVATE "quicklistpanel.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/public/vgui_controls/vgui_controls.cpp")


if( (NOT DEFINED NO_STEAM) )
    #Looks like we have to include libsteam_api
    message("building with steam_api")
	if( MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4 )
		target_link_libraries(${OUTBINNAME} ${LIBPUBLIC}/steam_api.lib)
	elseif( MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 8 )
		target_link_libraries(${OUTBINNAME} ${LIBPUBLIC}/steam_api64.lib)
	else()
		target_link_libraries(${OUTBINNAME} ${LIBPUBLIC}/libsteam_api.so)
	endif()
endif()

#Careful with link order here.
target_link_libraries(${OUTBINNAME} tier3_client vgui_controls_client dmxloader_client vstdlib_client tier2_client mathlib_client)