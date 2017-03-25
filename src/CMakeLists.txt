
add_subdirectory(common)
add_subdirectory(cpc)
add_subdirectory(sdltk)

add_executable(roland "${CMAKE_CURRENT_SOURCE_DIR}/roland.cpp")

target_link_libraries(roland common cpc sdltk ${SDL_LIBRARY})

install(TARGETS roland DESTINATION bin) # Add this for CPack to work


