#include <csono/socket.hpp>

#include <cstring>

#define DEBUG_IOSTREAM__NO
#ifdef DEBUG_IOSTREAM
	#pragma GCC warning "debug iostream"
	#include <iostream>
	#define DEBUG_OUT(debug_output) std::cout<<debug_output<<std::endl;
#else
	#define DEBUG_OUT(a)
#endif



namespace csono {

	inline namespace {

		inline char* cstr_copy(const char * src) {
			if(src == nullptr)  return nullptr;
			size_t size = ::strlen(src);
			char* retn = (char*) ::malloc(sizeof(char) * (size + 1));
			retn[size] = '\0';
			strncpy(retn, src, size);
			return retn;
		}


		#pragma GCC warning "temporary debug function"
		inline std::string debug_addrinfo(const addrinfo & ai) {
			return
					std::string("AF")+std::to_string(ai.ai_family) +
					" LEN"+std::to_string(ai.ai_addrlen) +
					" NAME:"+(ai.ai_canonname == nullptr? "null" : ai.ai_canonname);
		}


		void addrinfo_list_to_array(
				Address::NodeArray& array,
				const addrinfo * ai
		) {
			/* The algorithm passes through each node twice; this overhead
			 * is justified / ignored, because one would hope that a call to
			 * ::getaddrinfo(...) returns a list of less than 100 addresses,
			 * thus capping the number of iterations to a very low number. */

			const addrinfo * node = ai;
			size_t nodes_n = 0;

			// count the nodes
			while(node != nullptr) {
				++nodes_n;
				node = node->ai_next;
			}

			// put the nodes in a new array
			array.ptr = new Address::Node[nodes_n];
			array.size = nodes_n;
			node = ai;  nodes_n = 0;
			while(node != nullptr) {
				array.ptr[nodes_n] = Address::Node(*node);
				node = node->ai_next;
				++nodes_n;
			}
		}

	}


	Address::Address() { }


	Address::Address(Node node) {
		nodes.ptr = new Node[1];
		nodes.size = 1;
		*nodes.ptr = std::move(node);
	}


	Address::Address(const char * host, const char * service) {
		addrinfo* ai;
		int result = ::getaddrinfo(host, service, nullptr, &ai);

		if(result == 0) {
			addrinfo_list_to_array(nodes, ai);
			::freeaddrinfo(ai);
		}
	}


	uint16_t Address::port() const {
		if(! *this)  return 0;
		switch(family()) {
			case AF_INET:   return ::ntohs(((sockaddr_in*)  generic())->sin_port);
			case AF_INET6:  return ::ntohs(((sockaddr_in6*) generic())->sin6_port);
		}
		return 0;
	}

}
