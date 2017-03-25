
add_subdirectory(floppy)
add_subdirectory(z80)

include_directories(floppy)
include_directories(z80)

add_library(cpc STATIC colours.cpp cpc.cpp crtc.cpp gatearray.cpp keyboard.cpp memman.cpp ppi.cpp psg.cpp sound.cpp vdu.cpp)

target_link_libraries(cpc floppy z80)
