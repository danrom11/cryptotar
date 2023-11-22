cmake_minimum_required(VERSION 3.12)
<<<<<<< HEAD
project(cryptotar)
=======
project(test_cryptotarlib)
>>>>>>> lcryptotar.lib

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

<<<<<<< HEAD
set(SOURCES cryptotar.cpp)
add_executable(cryptotar ${SOURCES})
=======
set(SOURCES main.cpp)

add_library(cryptotar STATIC cryptotarlib.cpp TarHeader.hpp sha256.cpp)

add_executable(test_cryptotarlib ${SOURCES})

target_link_libraries(test_cryptotarlib cryptotar)
>>>>>>> lcryptotar.lib
