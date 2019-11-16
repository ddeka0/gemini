#pragma once
#include "Chord/include/common.hpp"
class GrpcAsyncServer {
public:
	GrpcAsyncServer();
	~GrpcAsyncServer();
	void Run();
protected:
private:
	void HandleRpcs();
	// This section of code might not compile
	// because we dont have compiled code for proto files yet
	// for example we dont have mathlib ready

	// mathlib::Math::AsyncService service_;
	// std::unique_ptr<ServerCompletionQueue> cq_;
	// std::unique_ptr<Server> server_;
};