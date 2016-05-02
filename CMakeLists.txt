cmake_minimum_required(VERSION 2.6)

project(ROLAND C CXX)

set(PACKAGE_TITLE "Roland" CACHE STRING "Package Title")
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
find_package(SDL_image REQUIRED)
include_directories(${SDL_INCLUDE_DIR})

# roland
set(DEBUG 1)
configure_file("${PROJECT_SOURCE_DIR}/files/config.h.in" "${PROJECT_SOURCE_DIR}/src/config.h")
add_definitions("-DHAVE_CONFIG_H")
configure_file("${PROJECT_SOURCE_DIR}/files/roland.spec.in" "${PROJECT_SOURCE_DIR}/roland.spec")

install(DIRECTORY "${CMAKE_SOURCE_DIR}/files/roms"  DESTINATION share/roland)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/files/disks" DESTINATION share/roland)

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


set(CPACK_CMAKE_GENERATOR "Kate - Unix Makefiles")
set(CPACK_GENERATOR "STGZ;TGZ;TZ")
set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_SOURCE_DIR}/build;${PACKAGE_NAME};ALL;/")
set(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME} ${PACKAGE_VERSION} ${CMAKE_BUILD_TYPE}")
set(CPACK_OUTPUT_CONFIG_FILE "${CMAKE_SOURCE_DIR}/build/CPackConfig.cmake")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Amstrad Emulator based on Caprice Source rewritten in C++")
set(CPACK_PACKAGE_EXECUTABLES "ccmake;CMake")
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
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "${CMAKE_SOURCE_DIR}/build/CPackSourceConfig.cmake")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PACKAGE_NAME}-${PACKAGE_VERSION}.src")
set(CPACK_SOURCE_STRIP_FILES "")
set(CPACK_STRIP_FILES "bin/roland")
set(CPACK_SYSTEM_NAME "Linux-i686")
set(CPACK_TOPLEVEL_TAG "Linux-i686")
set(CPACK_SOURCE_IGNORE_FILES "\\.directory;\\.git/;bin/;build/;doc/")

SET(CPACK_GENERATOR "RPM")
SET(CPACK_RPM_PACKAGE_MAINTAINER "Fred Klaus") #required
include(CPack)


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
message("  * ${PACKAGE_TITLE} ${PACKAGE_VERSION} ${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("  * CXX_FLAGS_DEBUG..: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
endif()
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message("  * CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
endif()
message("")
