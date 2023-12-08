cmake_minimum_required(VERSION 3.12)
project(test_cryptotarlib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

set(CMAKE_C_FLAGS_LINUX "${CMAKE_C_FLAGS_DEBUG} -DLINUX")
set(CMAKE_CXX_FLAGS_LINUX "${CMAKE_CXX_FLAGS_DEBUG} -DLINUX")

set(SOURCES main.cpp)

add_library(cryptotar STATIC cryptotarlib.cpp TarHeader.hpp sha256.cpp)

add_executable(test_cryptotarlib ${SOURCES})

target_link_libraries(test_cryptotarlib cryptotar)
