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



namespace {

	#pragma GCC warning "unneeded complexity"
	/* UNNEEDED COMPLEXITY:
	 * the function NEEDS to return a std::string, while ::inet_ntop(...)
	 * NEEDS to write on a C-style string buffer; there's no way (afaik)
	 * to let ::inet_ntop(...) write on the std::string itself, without
	 * copying the buffer - which will die instantly anyway.
	 * This is a pet-peeve nightmare. */
	std::string get_hostname(const addrinfo * ai) {
		if(ai == nullptr)  return "null";
		constexpr size_t MAX_LENGTH = 48;
		char* buffer = new char[MAX_LENGTH + 1];  buffer[MAX_LENGTH] = '\0';
		::inet_ntop(
				ai->ai_family,
				ai->ai_addr,
				buffer,
				MAX_LENGTH );
		std::string retn = buffer;
		delete[] buffer;
		return retn;
	}


	sockaddr* clone_sockaddr(const sockaddr * cpy, socklen_t len) {
		if(cpy == nullptr)  return nullptr;
		sockaddr* retn = reinterpret_cast<sockaddr*>(::malloc(len));
		DEBUG_OUT("CLONING " << reinterpret_cast<const void *>(cpy) << " TO " << reinterpret_cast<const void *>(retn))
		::memcpy(retn, cpy, len);
		return retn;
	}

	uint16_t ai_port(const addrinfo & ai) {
		switch(ai.ai_family) {
			case AF_INET:   return ntohs(((sockaddr_in*)  ai.ai_addr)->sin_port);
			case AF_INET6:  return ntohs(((sockaddr_in6*) ai.ai_addr)->sin6_port);
		}
		return 0;
	}

	std::string ai_fullname(const std::string & host, const addrinfo & ai) {
		uint16_t port = ai_port(ai);
		switch(ai.ai_family) {
			case AF_INET:   return host + ':' + std::to_string(port);
			case AF_INET6:  return '[' + host + "]:" + std::to_string(port);
		}
		return "unknown_host";
	}

}



namespace csono {

	Address::Node::Node():
			flags(0),
			family(AF_UNSPEC),
			addr_len(0),
			addr(nullptr),
			name(std::string()),
			fullname("null")
	{ }

	Address::Node::Node(const addrinfo & ai):
			flags(ai.ai_flags),
			family(ai.ai_family),
			addr_len(ai.ai_addrlen),
			addr(clone_sockaddr(ai.ai_addr, ai.ai_addrlen)),
			name(get_hostname(&ai)),
			fullname(ai_fullname(name, ai))
	{
		DEBUG_OUT(std::string("CREATING ") << reinterpret_cast<const void *>(addr))
	}


	Address::Node::Node(const Node & cpy):
			flags(cpy.flags),
			family(cpy.family),
			addr_len(cpy.addr_len),
			addr(clone_sockaddr(cpy.addr, cpy.addr_len)),
			name(cpy.name),
			fullname(cpy.fullname)
	{
		DEBUG_OUT(std::string("COPYING ") << reinterpret_cast<const void *>(cpy.addr))
	}


	Address::Node::Node(Node&& mov):
			flags(std::move(mov.flags)),
			family(std::move(mov.family)),
			addr_len(std::move(mov.addr_len)),
			addr(mov.addr),
			name(std::move(mov.name)),
			fullname(std::move(mov.fullname))
	{
		DEBUG_OUT(std::string("MOVING ") << reinterpret_cast<const void *>(mov.addr))
		mov.addr = nullptr;
		mov.addr_len = 0;
	}


	Address::Node& Address::Node::operator = (const Address::Node & cpy) {
		if(addr != nullptr)  ::free(addr);
		DEBUG_OUT(std::string("COPY-ASSIGNING ") << reinterpret_cast<const void *>(cpy.addr))

		flags = cpy.flags;
		family = cpy.family;
		addr_len = cpy.addr_len;
		addr = clone_sockaddr(cpy.addr, cpy.addr_len);
		name = cpy.name;
		fullname = cpy.fullname;

		return *this;
	}


	Address::Node& Address::Node::operator = (Address::Node&& mov) {
		if(addr != nullptr)  ::free(addr);
		DEBUG_OUT(std::string("MOVE-ASSIGNING ") << reinterpret_cast<const void *>(mov.addr))

		flags = std::move(mov.flags);
		family = std::move(mov.family);
		addr_len = std::move(mov.addr_len);
		addr = std::move(mov.addr);
		name = std::move(mov.name);
		fullname = std::move(mov.fullname);

		mov.addr = nullptr;  mov.addr_len = 0;
		return *this;
	}


	Address::Node::~Node() {
		DEBUG_OUT(std::string("DESTROYING ") << reinterpret_cast<const void *>(addr) << " (with FACTS and LOGIC)")
		if(addr != nullptr) {
			::free(addr);
			addr = nullptr;
			addr_len = 0;
		}
	}

}
