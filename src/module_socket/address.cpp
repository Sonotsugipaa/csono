#include <csono/socket.hpp>

#include <cstring>



namespace csono {

	Address::Address():
			size(0)
	{
		::memset(this, 0, sizeof value);
	}

	Address Address::v4(std::string host, uint16_t port) {
		Address retn;
		retn.value.v4.sin_family = AF_INET;
		retn.value.v4.sin_addr.s_addr = ::inet_addr(host.c_str());
		retn.value.v4.sin_port = htons(port);
		retn.size = sizeof(sockaddr_in);
		return retn;
	}

}
