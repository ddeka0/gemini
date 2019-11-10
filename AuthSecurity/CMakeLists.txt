cmake_minimum_required(VERSION 3.5)

include_directories(include/)
include_directories(../)

file(GLOB AUTH_SRC src/*.cpp)

add_library(libAUTH STATIC ${AUTH_SRC})

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -g")
# set(CRYPTO_LIBRARIES  "-lssl -lcrypto")