#define DEBUG_IOSTREAM__NO
#ifdef DEBUG_IOSTREAM
	#pragma GCC warning "debug iostream"
	#include <iostream>
	#define DEBUG_OUT(debug_output) std::cout<<"DEBUG >>> "<<debug_output<<std::endl;
#else
	#define DEBUG_OUT(a)
#endif



#include "args_args.cpp"
#include "args_argsparser.cpp"
