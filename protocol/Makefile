# This Makefile is for Creating grpc related protocol file headers
# plus the static library of the grpc .cpp files for rpc related codes
CXX = g++
CPPFLAGS += `pkg-config --cflags --libs protobuf grpc`
CXXFLAGS += -std=c++11 -g

GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

all: libmylib.a

libmylib.a: test.pb.o test.grpc.pb.o
	ar rcs libmylib.a $^

%.grpc.pb.cc: %.proto
	protoc --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.pb.cc: %.proto
	protoc --cpp_out=. $<

libs: libmylib.a

clean:
	rm -f *.o *.a *.pb.cc *.pb.h