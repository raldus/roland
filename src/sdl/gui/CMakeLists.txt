



add_subdirectory(font)

include_directories(font)
include_directories(${PROJECT_SOURCE_DIR}/src)



add_library(sdlgui STATIC fileselect.cpp)



target_link_libraries(sdlgui font common ${SDLIMAGE_LIBRARY})
