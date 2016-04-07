



add_subdirectory(filesys)

include_directories(${ROLAND_SOURCE_DIR})
include_directories(filesys)


add_library(common STATIC prefs.cpp)



target_link_libraries(common filesys)
