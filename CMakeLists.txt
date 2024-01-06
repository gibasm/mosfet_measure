cmake_minimum_required(VERSION 3.10)

project(fetmes C)

set(TARGET_NAME fetmes)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    src/main.c
    src/serial.c
    src/tcp.c
)

add_executable(${TARGET_NAME} ${SOURCE_FILES})
