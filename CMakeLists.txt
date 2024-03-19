cmake_minimum_required(VERSION 3.27)
project(ampere C)

set(SRC_FOLDER src)
set(CMAKE_C_STANDARD 99)

add_executable(${PROJECT_NAME} ${SRC_FOLDER}/${PROJECT_NAME}.c
        src/ampere.c)
