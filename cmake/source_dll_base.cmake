include("${CMAKE_MODULE_PATH}/common_functions.cmake")
include("${CMAKE_MODULE_PATH}/platform_dirs.cmake") #defines PLATSUBDIR
include("${CMAKE_MODULE_PATH}/source_base.cmake")

add_definitions(-DCSTRIKE15)

if(WIN32)
    set(OUTDLLEXT ".dll")
elseif(OSXALL)
    set(OUTDLLEXT ".dylib")
elseif(LINUXALL)
    set(OUTDLLEXT ".so")
endif()

MacroRequired(OUTDLLEXT ".so")

include("${CMAKE_MODULE_PATH}/source_dll_posix_base.cmake")
include("${CMAKE_MODULE_PATH}/source_video_base.cmake")
