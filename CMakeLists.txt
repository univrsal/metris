cmake_minimum_required(VERSION 2.8)
project(metris)

if(MSVC)
    find_path(SDL2_INCLUDE_DIRS SDL.h)
    find_library(SDL2_LIBRARIES SDL2)

    find_path(BASS_INCLUDE_DIRS bass.h)
    find_library(BASS_LIBRARIES bass)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
else()
    set(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")
    SET(GCC_FLAGS "-fpermissive")
    INCLUDE(FindPkgConfig)

    PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
    PKG_SEARCH_MODULE(BASS REQUIRED bass)
endif()


set(metris_SOURCES
    src/sdl_helper.cpp
    src/sdl_helper.hpp
    src/timer.cpp
    src/timer.hpp
    src/atlas.cpp
    src/atlas.hpp
    src/segment_display.cpp
    src/segment_display.hpp
    src/bass_handler.cpp
    src/bass_handler.hpp
    src/brick.cpp
    src/brick.hpp
    src/game_logic.cpp
    src/game_logic.hpp
    src/level.cpp
    src/level.hpp
    src/sprite.cpp
    src/sprite.hpp
    src/star.cpp
    src/star.hpp
    src/util.cpp
    src/util.hpp
    src/main.cpp)

add_executable(metris ${metris_SOURCES})

include_directories(${SDL2_INCLUDE_DIRS} ${BASS_INCLUDE_DIRS} ${SDLTTF2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${BASS_LIBRARIES})