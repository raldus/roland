



include_directories(${PROJECT_SOURCE_DIR}/src)


add_library(floppy STATIC cmdtable.cpp cmdtabledef.cpp drive.cpp fdc.cpp sector.cpp track.cpp)
