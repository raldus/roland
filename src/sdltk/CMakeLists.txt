
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cpc)
include_directories(${SDL_INCLUDE_DIR})

add_library(sdltk STATIC 
audio.cpp
border.cpp
button.cpp
color.cpp
canvas.cpp
canvasgl.cpp
canvasstd.cpp
exception.cpp
filelist.cpp
filelistitem.cpp
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

target_link_libraries(sdltk ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY})
