cmake_minimum_required(VERSION 3.5)

include_directories(include/)
include_directories(../)

file(GLOB CLIENT_SRC src/*.cpp)

add_library(libCLIENT STATIC ${CLIENT_SRC})

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -g")
# set(CRYPTO_LIBRARIES  "-lssl -lcrypto")