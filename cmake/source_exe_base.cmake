include("${CMAKE_MODULE_PATH}/platform_dirs.cmake") #defines PLATSUBDIR
include("${CMAKE_MODULE_PATH}/source_base.cmake")

if(POSIX)
    include("${CMAKE_MODULE_PATH}/source_exe_posix_base.cmake")
elseif(WIN32)
    include("${CMAKE_MODULE_PATH}/source_exe_posix_base.cmake")
elseif(OSXALL OR APPLE AND UNIX)
    include("${CMAKE_MODULE_PATH}/source_exe_posix_base.cmake")
else()
	message(FATAL_ERROR "OS Unsupported!")
endif()

include("${CMAKE_MODULE_PATH}/source_video_base.cmake")
