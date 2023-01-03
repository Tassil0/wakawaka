cmake_minimum_required(VERSION 3.21)
project(wakawaka)

set(CMAKE_C_STANDARD 11)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)

add_compile_options(-Wall -Wempty-body -Werror -Wstrict-prototypes -Werror=maybe-uninitialized -Warray-bounds -g -lefence)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

# Add the include directories of the (found) libraries.
include_directories(
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
)

file(GLOB SOURCES src/config.h src/types.h src/*.c src/*.h src/*/*.c src/*/*.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} src/colors.h src/text.c src/text.h)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)

Target_Link_Libraries(
        ${CMAKE_PROJECT_NAME}
        ${SDL2_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        m
)
