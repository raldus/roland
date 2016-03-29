



add_subdirectory(gui)

include_directories(gui)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/cpc)
include_directories(${SDL_INCLUDE_DIR})


add_library(sdltk STATIC keytrans.cpp)


target_link_libraries(sdltk sdlgui ${SDL_LIBRARY})
