cmake_minimum_required(VERSION 3.26)
project(PUD_O)

set(CMAKE_CXX_STANDARD 20)

set(APP_NAME pudo)

set(HEADERS Package.h PackageVersion.h PackagesHolder.h PackageUseDirHandler.h)
set(CPP_FILES main.cpp Package.cpp PackageVersion.cpp PackagesHolder.cpp PackageUseDirHandler.cpp)

if(NOT LINUX)
        message(FATAL_ERROR "You can not do this at all, CMake will exit.")
else()
        find_program(LSB_RELEASE_EXEC lsb_release)
        execute_process(COMMAND ${LSB_RELEASE_EXEC} -is
        OUTPUT_VARIABLE LSB_RELEASE_ID
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    # What if sys-apps/lsb-release doesnt installed?
    if(NOT LSB_RELEASE_ID STREQUAL "Gentoo")
        message(FATAL_ERROR "You should have got Gentoo Linux, CMake will exit.")
    else()
		add_executable(${APP_NAME} ${HEADERS} ${CPP_FILES})
    endif()
endif()
