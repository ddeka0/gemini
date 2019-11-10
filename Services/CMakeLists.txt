cmake_minimum_required(VERSION 3.5)

include_directories(include/)
include_directories(../)

file(GLOB SERVICES_SRC src/*.cpp)

add_library(libSERVICES STATIC ${SERVICES_SRC})

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -g")
# set(CRYPTO_LIBRARIES  "-lssl -lcrypto")