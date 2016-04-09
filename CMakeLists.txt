cmake_minimum_required(VERSION 2.6)

project(ROLAND)

set(CMAKE_BINARY_DIR      "${CMAKE_SOURCE_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR})

# C++ version
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. \
                    Please use a different C++ compiler.")
endif()

# C++ flags
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# SDL
find_package(SDL REQUIRED)
include_directories(${SDL_INCLUDE_DIR})

# roland
set(PACKAGE_STRING 1)
set(PACKAGE_NAME 1)

set(APP_NAME "Roland" CACHE STRING "App Name")
exec_program(git ARGS describe --abbrev=0 --tags OUTPUT_VARIABLE APP_VERSION)
set(APP_VERSION "${APP_VERSION}" CACHE STRING "App Version")

set(DEBUG 1)
configure_file("${PROJECT_SOURCE_DIR}/config.h.in" "${PROJECT_SOURCE_DIR}/src/config.h")
add_definitions("-DHAVE_CONFIG_H")

# cache-variables
# set(CACHE_VAR_CHECK CACHE STRING TestString)



include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/common")
include_directories("${PROJECT_SOURCE_DIR}/src/common/filesys")
include_directories("${PROJECT_SOURCE_DIR}/src/cpc")
include_directories("${PROJECT_SOURCE_DIR}/src/cpc/z80")
include_directories("${PROJECT_SOURCE_DIR}/src/cpc/floppy")
include_directories("${PROJECT_SOURCE_DIR}/src/sdl")
include_directories("${PROJECT_SOURCE_DIR}/src/sdl/gui")
include_directories("${PROJECT_SOURCE_DIR}/src/sdl/gui/font")

add_subdirectory(src)


# output
message("")
message("Package: ${ROLAND_NAME} ${ROLAND_VERSION}")
message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message("CMAKE_CXX_FLAGS_DEBUG:   ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
