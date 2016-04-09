cmake_minimum_required(VERSION 2.6)

project(ROLAND)

set(PROJECT_NAME "Roland" CACHE STRING "Project Name")
set(PACKAGE_NAME "roland" CACHE STRING "Package Name")
exec_program(git ARGS describe --abbrev=0 --tags OUTPUT_VARIABLE PACKAGE_VERSION)
set(PACKAGE_VERSION "${PACKAGE_VERSION}" CACHE STRING "Package Version")

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
set(DEBUG 1)
configure_file("${PROJECT_SOURCE_DIR}/files/config.h.in" "${PROJECT_SOURCE_DIR}/src/config.h")
add_definitions("-DHAVE_CONFIG_H")

# doxygen target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file("${CMAKE_SOURCE_DIR}/files/doxyfile.in" "${CMAKE_SOURCE_DIR}/build/doxyfile" @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} "${CMAKE_SOURCE_DIR}/build/doxyfile"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)


# uninstall target
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/build/CMakeModules)
configure_file(
    "${CMAKE_SOURCE_DIR}/files/uninstall.in"
    "${CMAKE_SOURCE_DIR}/build/uninstall"
    IMMEDIATE @ONLY)
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/build/uninstall)



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
message("  * ${PROJECT_NAME} ${PACKAGE_VERSION} ${CMAKE_BUILD_TYPE}")
message("  * CMAKE_CXX_FLAGS_DEBUG..: ${CMAKE_CXX_FLAGS_DEBUG}")
message("  * CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
message("")
