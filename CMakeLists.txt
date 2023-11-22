cmake_minimum_required(VERSION 3.12)
project(cryptotar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

set(SOURCES cryptotar.cpp)

add_library(cryptotarlib STATIC cryptotarlib.cpp TarHeader.hpp sha256.cpp)

add_executable(cryptotar ${SOURCES})

target_link_libraries(cryptotar cryptotarlib)
