cmake_minimum_required(VERSION 3.27)
project(ampere-tests C)

set(SRC_FOLDER ../src)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -pedantic")

include_directories(src/wm)

include_directories(${SRC_FOLDER}/wm ${SRC_FOLDER}/logging)

add_executable(
        ${PROJECT_NAME} test.c
        ${SRC_FOLDER}/wm/amp_wm.c
        ${SRC_FOLDER}/wm/amp_wm.c
        ${SRC_FOLDER}/logging/amp_logger.c
        ${SRC_FOLDER}/wm/layout/amp_column_layout.c
)

target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})