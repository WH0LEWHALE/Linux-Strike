include("${CMAKE_MODULE_PATH}/common_functions.cmake")

########source_lowest_base########
if(STATIC_LINK)
    add_definitions(-DSTATIC_LINK)
endif()
##################################

MacroRequired(SRCDIR)
MacroRequired(_DLL_EXT)

set(LIBPUBLIC "${SRCDIR}/lib/public${PLATSUBDIR}") #this is where static libs are
#link_directories(${LIBPUBLIC}) #add to search path for linker - lwss: use the project name instead of linking the files manually.
set(LIBCOMMON "${SRCDIR}/lib/common${PLATSUBDIR}")
set(DEVTOOLS "${SRCDIR}/devtools")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    message(STATUS "Build type not specified")
endif(NOT CMAKE_BUILD_TYPE)

#-Werror=return-type - Set these warnings to ERRORS because they can ruin your stack/day
if(NOT WIN32)
	set(LINUX_FLAGS_COMMON " -ffast-math -march=native -Wno-invalid-offsetof -Wno-ignored-attributes -Wno-enum-compare -Werror=return-type ")
	set(LINUX_DEBUG_FLAGS " -ggdb -g1") #3 -fno-eliminate-unused-debug-symbols
endif()

if( MSVC )
	# Enable String Pooling: Yes (/GF)
	# Build with Multiple Processes (/MP)
	# Buffer Security Check: No (/GS-)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GF /MP /GS-")
	
	# @ssrobins at gitlab.kitware.com
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT>) #--|
endif()

#$Configuration "Debug"
if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    message(STATUS "Building in Debug mode")
    add_definitions(-DDEBUG -D_DEBUG -DDBGFLAG_ASSERT)
    if( OSXALL )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-2 -g2 -Og -march=native")
    elseif( LINUXALL )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og ${LINUX_DEBUG_FLAGS} ${LINUX_FLAGS_COMMON}")
    endif()
#$Configuration "Release"
else()
    message(STATUS "Building in Release mode")
	if( MSVC AND NOT CMAKE_BUILD_TYPE STREQUAL "DEBUG" )
		# Still generate pdbs
		add_compile_options("/Zi")
		add_link_options("/DEBUG:FASTLINK")
	endif()
    add_definitions(-DNDEBUG)
    if( OSXALL )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-2 -g2 -O2 -march=native")
    elseif( LINUXALL )
        if( NO_GCC_OPTIMIZE )
            message("^^ Not Setting -O for Target")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LINUX_DEBUG_FLAGS} ${LINUX_FLAGS_COMMON}")
        else()
            if(CMAKE_SYSTEM_PROCESSOR STREQUAL "e2k")
                # O3 on mcst-lcc approximately equal to O2 at gcc X86/ARM
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 ${LINUX_FLAGS_COMMON}")
            else()
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 ${LINUX_DEBUG_FLAGS} ${LINUX_FLAGS_COMMON}")
            endif()
        endif()
    endif()
endif()

#$Compiler
include_directories("${SRCDIR}/common")
include_directories("${SRCDIR}/public")
include_directories("${SRCDIR}/public/tier0")
include_directories("${SRCDIR}/public/tier1")
if(NOT WIN32)
	add_definitions(-DGNUC -DPOSIX -DCOMPILER_GCC)
	elseif(MSVC)
	add_definitions(-DCOMPILER_MSVC)
endif()
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	add_definitions(-DPLATFORM_64BITS)
	if(MSVC)
		add_definitions(-DCOMPILER_MSVC64)
	endif()
else()
	if(MSVC)
		add_definitions(-DCOMPILER_MSVC32)
	endif()
endif()
add_definitions(-DMEMOVERRIDE_MODULE=${PROJECT_NAME} -D_DLL_EXT=${_DLL_EXT})
if(DEDICATED)
    add_definitions(-DDEDICATED)
endif()
if(OSXALL)
    add_definitions(-D_OSX -DOSX -D_DARWIN_UNLIMITED_SELECT -DFD_SETSIZE=10240)
endif()

if(LINUXALL)
    if( USE_ASAN )
        add_definitions(-DUSE_ASAN)
        add_compile_options(-fno-omit-frame-pointer -fsanitize=address -fsanitize-recover=address)
        add_link_options(-fsanitize=address -fsanitize-recover=address)
    endif()

    #add_definitions(-D_LINUX -DLINUX)
    if( DONT_DOWNGRADE_ABI )
        message(STATUS "KEEPING CXX11 ABI FOR PROJECT")
    else()
        #message(STATUS "DOWNGRADING CXX11 ABI")
        #disable cpp11 ABI so libraries <gcc 5 will work
        add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
    endif()
endif()
if(POSIX)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden) #$SymbolVisibility	"hidden"
    add_definitions(-DPOSIX -D_POSIX)
endif()
if(OSX64)
    add_definitions(-DPLATFORM_64BITS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64")
endif()

if(NOT IS_LIB_PROJECT)
    #set(ConfigurationType "Application (.exe)") #not used

    #$Linker
    if(OSX64)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -arch x86_64")
    endif()

    #$Folder	"Link Libraries"
    if( NOSTINKYLINKIES )
        message(STATUS "skipping stinky linkie")
    else()
        link_libraries("libtier0_client")
        link_libraries("tier1_client")
        link_libraries("interfaces_client")
        #include_directories("vstdlib")
    endif()
endif()
