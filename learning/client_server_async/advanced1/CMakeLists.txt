cmake_minimum_required(VERSION 3.5)

project(client-server)
message(STATUS "Compiling Network Function : ${PROJECT_NAME}")
set(CMAKE_CXX_STANDARD 11)

message(STATUS "Project Directory: ${PROJECT_SOURCE_DIR}")


set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIB_FILE ${CMAKE_CURRENT_SOURCE_DIR}/proto_lib/libmylib.a)



add_custom_target(libmylib.a ALL
					COMMAND make
					WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/proto_lib
					COMMENT "grpc headers and libraries are generating..."
)



add_executable(server server.cpp)


set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -g")
set(BASIC_LIBRARIES "-lpthread")
set(CMAKE_LINK_WHAT_YOU_USE ON)


find_library(proto_location NAMES protobuf PATHS ${LIBDIR})
find_library(grpc_location NAMES grpc++ PATHS ${LIBDIR})
find_library(grpc_reflection_location NAMES grpc++_reflection PATHS ${LIBDIR})
find_library(dl_location NAMES dl PATHS ${LIBDIR})
find_library(grpc_location2 NAMES grpc PATHS ${LIBDIR})

if(proto_location)
  message(STATUS "protobuf library found at " ${proto_location})
endif()
if(grpc_location)
  message(STATUS "grpc++ found at " ${grpc_location})
endif()
if(grpc_reflection_location)
  message(STATUS "grpc_reflection found at " ${grpc_reflection_location})
endif()
if(dl_location)
  message(STATUS "dl found at " ${dl_location})
endif()
if(grpc_location2)
  message(STATUS "grpc found at " ${grpc_location2})
endif()

target_link_libraries(server ${proto_location}
							${LIB_FILE}
							${grpc_location}
							${grpc_reflection_location}
							${dl_location}
							${grpc_location2}
							${BASIC_LIBRARIES}
)

add_executable(client client.cpp)
target_link_libraries(client ${proto_location}
							${LIB_FILE}
							${grpc_location}
							${grpc_reflection_location}
							${dl_location}
							${grpc_location2}
							${BASIC_LIBRARIES}
)