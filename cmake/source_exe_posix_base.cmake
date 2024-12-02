include("${CMAKE_MODULE_PATH}/common_functions.cmake")
include("${CMAKE_MODULE_PATH}/source_posix_base.cmake")

MacroRequired(SRCDIR)
MacroRequired(OUTBINNAME)
MacroRequired(OUTBINDIR)

set( IS_LIB_PROJECT "1")

add_definitions( -DEXENAME=${OUTBINNAME} )

if(WIN32 AND MSVC)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE") # Use more than 2 gigabytes of memory on windows
	set(CMAKE_WIN32_EXECUTABLE ON) # Sets subsystem to WINDOWS
endif()

add_executable(${OUTBINNAME})

if(WIN32)
	set_target_properties(${OUTBINNAME} PROPERTIES OUTPUT_NAME "${OUTBINNAME}.exe")
else()
	set_target_properties(${OUTBINNAME} PROPERTIES OUTPUT_NAME "${OUTBINNAME}")
endif()
set_target_properties(${OUTBINNAME} PROPERTIES SUFFIX "")
set_target_properties(${OUTBINNAME} PROPERTIES PREFIX "")

message("Adding executable target: ${OUTBINNAME}\n")

set_target_properties( ${OUTBINNAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${OUTBINDIR}"
        LIBRARY_OUTPUT_DIRECTORY "${OUTBINDIR}"
        RUNTIME_OUTPUT_DIRECTORY "${OUTBINDIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTBINDIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTBINDIR}"
        )

if( NOSKELETONBASE )
    message(STATUS "Not including Skeleton base.")
else()
    target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/public/tier0/memoverride.cpp")
endif()
