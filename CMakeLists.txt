cmake_minimum_required(VERSION 3.18)
project(example)
include_directories(./include/)
file(GLOB_RECURSE SHELL_LIB_SRC
        ./src/*.c ./port/*.c
        )

add_library(SHELL_LIB STATIC ${SHELL_LIB_SRC})
add_executable(temp ./demo/main.c)
target_link_libraries(temp SHELL_LIB)
