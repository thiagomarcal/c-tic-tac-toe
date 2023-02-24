cmake_minimum_required(VERSION 3.24)
project(tic-tac-toe C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(src)
include_directories(include)
link_directories(lib)

add_executable(tic-tac-toe
        src/glad.c
        src/main.c)

target_include_directories(tic-tac-toe PUBLIC include)
target_link_libraries(tic-tac-toe LINK_PUBLIC SDL2main SDL2 freetype)



