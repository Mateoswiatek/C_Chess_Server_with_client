cmake_minimum_required(VERSION 3.24)
project(1_KLIENT_SZACHY C)

set(CMAKE_C_STANDARD 99)

link_libraries(ws2_32 wsock32)

add_executable(1_KLIENT_SZACHY main.c)
