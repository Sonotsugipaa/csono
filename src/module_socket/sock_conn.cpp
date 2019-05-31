#include <csono/socket.hpp>



namespace csono {

	Socket::Socket(int fd):
			sock_fd(fd)
	{ }

	Socket::Socket(int addr_type, int socket_type, int protocol):
			sock_fd(::socket(addr_type, socket_type, protocol))
	{ }

	Socket::Socket(Socket&& mov):
			sock_fd(mov.sock_fd)
	{ mov.sock_fd = -1; }

	Socket::Tcp::Tcp(int af): Socket::Socket(af, SOCK_STREAM, IPPROTO_TCP) { }
	Socket::Udp::Udp(int af): Socket::Socket(af, SOCK_DGRAM,  IPPROTO_UDP) { }


	Socket::~Socket() {
		if(sock_fd != -1) {
			::close(sock_fd);  sock_fd = -1;
		}
	}


	Socket& Socket::operator = (Socket&& mov) {
		sock_fd = mov.sock_fd;  mov.sock_fd = -1;
		return *this;
	}


#pragma GCC warning "Socket::bind(uint16_t) only binds correctly on IPv4 sockets"
	bool Socket::bind(uint16_t port) {
		if(sock_fd == -1)  return false;
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = ::htonl(INADDR_ANY);
		sin.sin_port = ::htons(port);
		return 0 == ::bind(sock_fd, (sockaddr*) &sin, sizeof(sockaddr_in));
	}


	bool Socket::connect(Address rem) {
		if(sock_fd == -1)  return false;
		return -1 != ::connect(sock_fd, rem.generic(), rem.size);
	}

	bool Socket::listen(unsigned int backlog) {
		if(sock_fd == -1)  return false;
		return -1 != ::listen(sock_fd, backlog);
	}


	Connection Socket::accept() {
		if(sock_fd != -1) {
			Address retn_addr;
			int retn_sock = ::accept(sock_fd, retn_addr.generic(), &retn_addr.size);
			if(retn_sock == -1)  return Connection();
			return Connection(retn_addr, retn_sock);
		}
		return Connection();
	}


	void Socket::close() { this->~Socket(); }


	ssize_t Socket::read(void* dest, size_t max, unsigned int flags) {
		if(sock_fd == -1)  return -1;
		return ::recv(sock_fd, dest, max, flags);
	}

	ssize_t Socket::write(const void * src, size_t size, unsigned int flags) {
		if(sock_fd == -1)  return -1;
		return ::send(sock_fd, src, size, flags);
	}

}


namespace csono {

	Connection::Connection():
			_address(), _remote_socket(-1), _unique()
	{ }

	Connection::Connection(Address address, int sock_fd):
			_address(address), _remote_socket(sock_fd), _unique()
	{ }

}
