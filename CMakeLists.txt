cmake_minimum_required(VERSION 2.8)

project(ROLAND CXX C)

math(EXPR CMAKE_BITS ${CMAKE_SIZEOF_VOID_P}*8)

set(PACKAGE_TITLE "Roland" CACHE STRING "Package Title")
set(PACKAGE_NAME  "roland" CACHE STRING "Package Name")
set(PACKAGE_VERSION "${PACKAGE_VERSION}" CACHE STRING "Package Version")
execute_process(COMMAND git show --tags --quiet --reverse "--pretty=format:%ad %n[view tag](https://github.com/raldus/roland/commit/%H) %n%n***%n" OUTPUT_FILE "${CMAKE_SOURCE_DIR}/CHANGELOG.md")
execute_process(COMMAND git describe --abbrev=0 --tags --always OUTPUT_VARIABLE PACKAGE_VERSION)
string(REGEX REPLACE "\n$" "" PACKAGE_VERSION "${PACKAGE_VERSION}")



set(ROLAND_ROOTDIR "${CMAKE_SOURCE_DIR}/assets")

set(ROLAND_DATADIR "${ROLAND_ROOTDIR}/app")
set(ROLAND_ROMDIR  "${ROLAND_ROOTDIR}/roms")
set(ROLAND_DISKDIR "${ROLAND_ROOTDIR}/disks")
set(ROLAND_AMSROM  "${ROLAND_ROOTDIR}/roms/amsdos.rom")
set(ROLAND_CPCROM  "${ROLAND_ROOTDIR}/roms/cpc6128.rom")

#set(CMAKE_BINARY_DIR       "${CMAKE_SOURCE_DIR}/bin" CACHE STRING "Build Dir")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" CACHE STRING "Executable Output Path")
set(LIBRARY_OUTPUT_PATH    "${CMAKE_BINARY_DIR}/bin" CACHE STRING "Library Output Path")

if (WIN32)
    set(_WIN32 TRUE CACHE BOOL "Win32")
endif (WIN32)

if (MSVC)
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /BUILD_VERBOSE_FLAGS:lib")
    set(_WIN32 TRUE BOOL)

    if (${CMAKE_GENERATOR} MATCHES "Visual Studio 14 2015")
        if (${CMAKE_SIZEOF_VOID_P} EQUAL "8")
            message(FATAL_ERROR "SDL Prebuild libs for 64bit not available")
            set(SDL_LIBRARY "${CMAKE_SOURCE_DIR}/3rdparty/sdl-vc2015/lib/x64/SDL.lib")
            set(SDL_LIBRARY "${SDL_LIBRARY};${CMAKE_SOURCE_DIR}/3rdparty/sdl-vc2015/lib/x64/SDLmain.lib")
            set(SDLIMAGE_LIBRARY "${CMAKE_SOURCE_DIR}/3rdparty/sdl-vc2015/lib/x64/SDL_image.lib")
        else()
            set(SDL_LIBRARY "${CMAKE_SOURCE_DIR}/3rdparty/sdl-vc2015/lib/x86/SDL.lib")
            set(SDL_LIBRARY "${SDL_LIBRARY};${CMAKE_SOURCE_DIR}/3rdparty/sdl-vc2015/lib/x86/SDLmain.lib")
            set(SDLIMAGE_LIBRARY "${CMAKE_SOURCE_DIR}/3rdparty/sdl-vc2015/lib/x86/SDL_image.lib")
        endif()
        set(SDL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/sdl-vc2015/include")
    else()
        if (${CMAKE_SIZEOF_VOID_P} EQUAL "8")
            set(SDL_LIBRARY "${CMAKE_SOURCE_DIR}/3rdparty/sdl-vc/lib/x64/SDL.lib")
            set(SDL_LIBRARY "${SDL_LIBRARY};${CMAKE_SOURCE_DIR}/3rdparty/sdl-vc/lib/x64/SDLmain.lib")
            set(SDLIMAGE_LIBRARY "${CMAKE_SOURCE_DIR}/3rdparty/sdl-vc/lib/x64/SDL_image.lib")
        else()
            set(SDL_LIBRARY "${CMAKE_SOURCE_DIR}/3rdparty/sdl-vc/lib/x86/SDL.lib")
            set(SDL_LIBRARY "${SDL_LIBRARY};${CMAKE_SOURCE_DIR}/3rdparty/sdl-vc/lib/x86/SDLmain.lib")
            set(SDLIMAGE_LIBRARY "${CMAKE_SOURCE_DIR}/3rdparty/sdl-vc/lib/x86/SDL_image.lib")
        endif()
        set(SDL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/sdl-vc/include")
    endif()
endif (MSVC)

if(UNIX)
	# C++ version
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	if(COMPILER_SUPPORTS_CXX11)
	    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	elseif(COMPILER_SUPPORTS_CXX0X)
	    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	else()
	    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. \
	                    Please use a different C++ compiler.")
	endif()

    # C++ flags
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -Wall")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")


    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" AND ${CMAKE_SIZEOF_VOID_P} EQUAL "8")
        set(LIB_SUFFIX "64")
    endif()
    
    # eclipse stop on error
    add_definitions(-Wfatal-errors)
    # be BUILD_VERBOSE_FLAGS
    if(${BUILD_VERBOSE_FLAGS})
        add_definitions(-Wall -Wextra -pedantic) # -Weffc++
    endif()


    # SDL
    find_package(SDL REQUIRED)
    find_package(SDL_image REQUIRED)
    include_directories("${SDL_INCLUDE_DIR}")

endif(UNIX)

# sdl
include_directories("${SDL_INCLUDE_DIR}")
message(STATUS "SDL_INCLUDE_DIR:  ${SDL_INCLUDE_DIR}")
message(STATUS "SDL_LIBRARY:      ${SDL_LIBRARY}")
message(STATUS "SDLIMAGE_LIBRARY: ${SDLIMAGE_LIBRARY}")

# roland
option(BUILD_VERBOSE_FLAGS "BUILD_VERBOSE_FLAGS" FALSE)
set(DEBUG 1)
add_definitions("-DHAVE_CONFIG_H")
configure_file("${PROJECT_SOURCE_DIR}/assets/config.h.in" 
               "${PROJECT_SOURCE_DIR}/src/config.h")
configure_file("${PROJECT_SOURCE_DIR}/assets/roland.spec.in" 
               "${CMAKE_BINARY_DIR}/roland.spec")

install(DIRECTORY "${ROLAND_ROMDIR}"  DESTINATION share/roland)
install(DIRECTORY "${ROLAND_DISKDIR}" DESTINATION share/roland)
install(DIRECTORY "${ROLAND_DATADIR}" DESTINATION share/roland)

# doxygen target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file("${CMAKE_SOURCE_DIR}/assets/doxyfile.in" 
                   "${CMAKE_BINARY_DIR}/doxyfile" @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/doxyfile"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)

# uninstall target
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_BINARY_DIR}/CMakeModules)
configure_file(
    "${CMAKE_SOURCE_DIR}/assets/uninstall.in"
    "${CMAKE_BINARY_DIR}/uninstall"
    IMMEDIATE @ONLY)
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/uninstall)

SET(ROLAND_DESKTOP_DIR  "${PROJECT_SOURCE_DIR}/assets/app")
SET(ROLAND_PIXMAPS_DIR  "${PROJECT_SOURCE_DIR}/assets/app")

set(CPACK_CMAKE_GENERATOR "Unix Makefiles")
set(CPACK_GENERATOR "STGZ;TGZ;TZ")
set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR};${PACKAGE_NAME};ALL;/")
set(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME} ${PACKAGE_VERSION} ${CMAKE_BUILD_TYPE}")
set(CPACK_OUTPUT_CONFIG_FILE "${CMAKE_BINARY_DIR}/CPackConfig.cmake")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Amstrad Emulator based on Caprice Source rewritten in C++")
set(CPACK_PACKAGE_EXECUTABLES "${PACKAGE_NAME};${PACKAGE_TITLE}")
set(CPACK_PACKAGE_FILE_NAME "${PACKAGE_NAME}-${PACKAGE_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PACKAGE_NAME}-${PACKAGE_VERSION}")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${PACKAGE_NAME}-${PACKAGE_VERSION}")
set(CPACK_PACKAGE_NAME ${PACKAGE_NAME})
set(CPACK_PACKAGE_CONTACT "Fred Klaus <development@fkweb.de")
set(CPACK_PACKAGE_VENDOR  "Fred Klaus <development@fkweb.de")
set(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README  "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_SOURCE_GENERATOR "TBZ2;RPM")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "${CMAKE_BINARY_DIR}/CPackSourceConfig.cmake")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PACKAGE_NAME}-${PACKAGE_VERSION}.src")
set(CPACK_SOURCE_STRIP_FILES "")
set(CPACK_STRIP_FILES "${EXECUTABLE_OUTPUT_PATH}/roland")
set(CPACK_SYSTEM_NAME "x86_64")
set(CPACK_TOPLEVEL_TAG "Linux-x86_64")
set(CPACK_SOURCE_IGNORE_FILES ".directory;.git/;3rdparty/;build/;doc/")

set(CPACK_GENERATOR "RPM")
set(CPACK_RPM_PACKAGE_MAINTAINER "Fred Klaus") #required
set(CPACK_RPM_PACKAGE_REQUIRES "SDL >= 1.2")
set(CPACK_RPM_PACKAGE_GROUP "System/Emulators/PC")
include(CPack)


include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/common")
include_directories("${PROJECT_SOURCE_DIR}/src/common/filesys")
include_directories("${PROJECT_SOURCE_DIR}/src/cpc")
include_directories("${PROJECT_SOURCE_DIR}/src/cpc/z80")
include_directories("${PROJECT_SOURCE_DIR}/src/cpc/floppy")
include_directories("${PROJECT_SOURCE_DIR}/src/sdltk")


add_subdirectory(src)




# output
message("")
message("  * ${PACKAGE_TITLE} ${PACKAGE_VERSION} ${CMAKE_BUILD_TYPE} ${CMAKE_BITS}bit")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("  * CXX_FLAGS_DEBUG..: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
endif()
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message("  * CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
endif()
message("")
