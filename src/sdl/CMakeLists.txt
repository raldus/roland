

add_subdirectory(gui)

include_directories(gui)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/cpc)
include_directories(${SDL_INCLUDE_DIR})


add_library(sdltk STATIC 
audio.cpp
border.cpp
button.cpp
color.cpp
draw.cpp
drawgl.cpp
drawstd.cpp
exception.cpp
fileselect.cpp
glfuncs.cpp
gui.cpp
image.cpp
imagefont.cpp
keytrans.cpp
label.cpp
list.cpp
listitem.cpp
video.cpp
videogl.cpp
videostd.cpp
widget.cpp)


target_link_libraries(sdltk sdlgui ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY})
