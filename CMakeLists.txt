cmake_minimum_required (VERSION 3.5)
set(CMAKE_BUILD_TYPE Debug)

# set project name and C++ compiler version
project(chord)
message(STATUS "Compiling Network Function : ${PROJECT_NAME}")
set (CMAKE_CXX_STANDARD 17)

message(STATUS "Project Directory: ${PROJECT_SOURCE_DIR}")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# add all necessary libs
add_subdirectory(AuthSecurity bin_auth)
add_subdirectory(Node bin_node)
add_subdirectory(Services bin_services)

# add all include paths
include_directories(${PROJECT_SOURCE_DIR})
include_directories(Chord/include/)


# add all src files
file(GLOB CHORD_FILES Chord/src/*.cpp)

# add all config files
# configure_file(<source_location>/file_name.json
# 		${PROJECT_SOURCE_DIR}/<file_name>.json COPYONLY)


add_executable(${PROJECT_NAME}
    ${CHORD_FILES}
)

# add_definitions(-DBOOST_LOG_DYN_LINK)

set(BASIC_LIBRARIES "-lpthread")
# set(REST_LIBRARIES "-lcpprest -lyaml-cpp")
# set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -g  -fsanitize=address")
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -g")

target_link_libraries(${PROJECT_NAME} libNODE libSERVICES
                        libAUTH 
                        ${BASIC_LIBRARIES})

