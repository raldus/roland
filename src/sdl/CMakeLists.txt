

add_subdirectory(gui)

include_directories(gui)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/cpc)
include_directories(${SDL_INCLUDE_DIR})


add_library(sdltk STATIC 
        image.cpp imagefont.cpp color.cpp exception.cpp gui.cpp draw.cpp drawgl.cpp drawstd.cpp glfuncs.cpp video.cpp videogl.cpp videostd.cpp keytrans.cpp)


target_link_libraries(sdltk sdlgui ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY})
