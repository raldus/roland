
add_subdirectory(common)
add_subdirectory(cpc)
add_subdirectory(sdl)

add_executable(roland "${PROJECT_SOURCE_DIR}/src/roland.cpp")

target_link_libraries(roland common cpc sdltk ${SDL_LIBRARY})

#set_property(TARGET roland PROPERTY VERSION "${PACKAGE_VERSION}")

install(PROGRAMS ${CMAKE_BINARY_DIR}/roland DESTINATION bin)



#add_library(filesys STATIC directory.cpp fileinfo.cpp filename.cpp)


