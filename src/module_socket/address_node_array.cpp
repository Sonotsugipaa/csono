#include <csono/socket.hpp>

#include <cstring>

#define DEBUG_IOSTREAM__NO
#ifdef DEBUG_IOSTREAM
	#pragma GCC warning "debug iostream"
	#include <iostream>
	#define DEBUG_OUT(debug_output) std::cout<<"DEBUG >>> "<<debug_output<<std::endl;
#else
	#define DEBUG_OUT(a)
#endif



namespace csono {

	Address::NodeArray::NodeArray():
			ptr(&Address::Node::null), size(0)
	{
		DEBUG_OUT("NODEARRAY NULL PTR" << ptr << " SIZE" << size)
	}


	Address::NodeArray::NodeArray(const NodeArray & cpy):
			ptr((cpy.size == 0)?  nullptr : new Node[cpy.size]),
			size(cpy.size)
	{
		for(unsigned i=0; i<size; ++i)
			ptr[i] = cpy.ptr[i];
	}


	Address::NodeArray::NodeArray(NodeArray&& mov):
			ptr(std::move(mov.ptr)),
			size(std::move(mov.size))
	{
		mov.ptr = nullptr;  mov.size = 0;
	}


	Address::NodeArray::~NodeArray() {
		DEBUG_OUT("NODEARRAY DEALLOC PTR" << ptr << " SIZE" << size)
		if(size > 0) {
			delete[] ptr;
			size = 0;
			ptr = nullptr;
		}
	}


	Address::NodeArray& Address::NodeArray::operator = (const NodeArray & cpy) {
		delete[] ptr;  ptr = new Node[cpy.size];
		size = cpy.size;

		for(unsigned i=0; i<size; ++i)
			ptr[i] = cpy.ptr[i];

		return *this;
	}


	Address::NodeArray& Address::NodeArray::operator = (NodeArray&& mov) {
		delete[] ptr;  ptr = mov.ptr;
		size = mov.size;

		mov.ptr = nullptr;  mov.size = 0;

		return *this;
	}

}
