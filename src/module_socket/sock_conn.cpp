#include <csono/socket.hpp>

#include <cstring>



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


	Socket::~Socket() {
		if(sock_fd != -1) {
			::close(sock_fd);  sock_fd = -1;
		}
	}


	Socket& Socket::operator = (Socket&& mov) {
		sock_fd = mov.sock_fd;  mov.sock_fd = -1;
		return *this;
	}


	bool Socket::bind(Address local) {
		if(sock_fd == -1)  return false;
		return 0 == ::bind(sock_fd, local.generic(), local.generic_size());
	}


	bool Socket::connect(Address rem) {
		if(sock_fd == -1)  return false;
		return -1 != ::connect(sock_fd, rem.generic(), rem.generic_size());
	}

	bool Socket::listen(unsigned int backlog) {
		if(sock_fd == -1)  return false;
		return -1 != ::listen(sock_fd, backlog);
	}


	Connection Socket::accept() {
		if(sock_fd != -1) {
			sockaddr recv_addr;  socklen_t recv_addr_size = 0;
			::memset(&recv_addr, 0, sizeof(recv_addr));
			int retn_sock = ::accept(sock_fd, &recv_addr, &recv_addr_size);
			if(retn_sock == -1)  return Connection();
			return Connection(Address(/* I don't know. I have no idea. */), retn_sock);
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
