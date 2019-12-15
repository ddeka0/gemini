LDFLAGS = -L/usr/local/lib `pkg-config --libs protobuf grpc++`\
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
           -ldl -lgrpc -lpthread


CXX = g++
CPPFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++17 -g

GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

all: libmylib.a

# client: math.pb.o math.grpc.pb.o client.o
# 	$(CXX) $^ $(LDFLAGS) -o $@

# server: math.pb.o math.grpc.pb.o server.o
# 	$(CXX) $^ $(LDFLAGS) -o $@

libmylib.a: math.pb.o math.grpc.pb.o
	ar rcs libmylib.a $^

%.grpc.pb.cc: %.proto
	protoc --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.pb.cc: %.proto
	protoc --cpp_out=. $<

libs: libmylib.a

clean:
	rm -f *.o *.a *.pb.cc *.pb.h client server
