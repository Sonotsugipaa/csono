#include <csono/socket.hpp>



namespace csono {

	Socket::Socket(int fd):
			fd(fd)
	{ }

	Socket::Socket(int addr_type, int socket_type, int protocol):
			fd(::socket(addr_type, socket_type, protocol))
	{ }

	Socket::Socket(Socket&& mov):
			fd(mov.fd)
	{ mov.fd = -1; }


	Socket::~Socket() {
		if(fd != -1) {
			::close(fd);  fd = -1;
		}
	}


	Socket& Socket::operator = (Socket&& mov) {
		fd = mov.fd;  mov.fd = -1;
		return *this;
	}


	bool Socket::bind(uint16_t port) {
		if(fd == -1)  return false;
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = ::htonl(INADDR_ANY);
		sin.sin_port = ::htons(port);
		return 0 == ::bind(fd, (sockaddr*) &sin, sizeof(sockaddr_in));
	}


	bool Socket::connect(Address rem) {
		if(fd == -1)  return false;
		return -1 != ::connect(fd, rem.generic(), rem.size);
	}

	bool Socket::listen(unsigned int backlog) {
		if(fd == -1)  return false;
		return -1 != ::listen(fd, backlog);
	}


	Connection Socket::accept() {
		if(fd != -1) {
			Address retn_addr;
			int retn_sock = ::accept(fd, retn_addr.generic(), &retn_addr.size);
			if(retn_sock == -1)  return Connection();
			return Connection(retn_addr, retn_sock);
		}
		return Connection();
	}


	void Socket::close() { this->~Socket(); }


	ssize_t Socket::read(void* dest, size_t max) {
		if(fd == -1)  return -1;
		return ::recv(fd, dest, max, 0);
	}

	ssize_t Socket::write(const void * src, size_t size) {
		if(fd == -1)  return -1;
		return ::send(fd, src, size, 0);
	}

}


namespace csono {

	Connection::Connection():
			_address(), _remote_socket(-1), _unique()
	{ }

	Connection::Connection(Address address, int fd):
			_address(address), _remote_socket(fd), _unique()
	{ }

}
