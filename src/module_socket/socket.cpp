#include <csono/socket.hpp>

#include <cstring>



namespace csono {

	Socket::Socket(int fd, Address local, Address remote):
			sock_fd(fd),
			sock_type(0), sock_protocol(0),
			bound_addr(std::move(local)), connected_addr(std::move(remote))
	{ }

	Socket::Socket(int addr_type, int socket_type, int protocol):
			sock_fd(::socket(addr_type, socket_type, protocol)),
			sock_type(socket_type),
			sock_protocol(protocol),
			bound_addr(), connected_addr()
	{ }

	Socket::Socket(Socket&& mov):
			sock_fd(std::move(mov.sock_fd)),
			sock_type(std::move(mov.sock_type)),
			sock_protocol(std::move(mov.sock_protocol)),
			bound_addr(std::move(mov.bound_addr)),
			connected_addr(std::move(mov.connected_addr))
	{ mov.sock_fd = -1; }


	Socket::~Socket() {
		if(sock_fd != -1) {
			::close(sock_fd);  sock_fd = -1;
			bound_addr = Address();
			connected_addr = Address();
		}
	}


	Socket& Socket::operator = (Socket&& mov) {
		this->~Socket();
		sock_fd = mov.sock_fd;  mov.sock_fd = -1;
		sock_type = mov.sock_type;  mov.sock_type = 0;
		sock_protocol = mov.sock_protocol;  mov.sock_protocol = 0;
		bound_addr = std::move(mov.bound_addr);
		connected_addr = std::move(mov.connected_addr);
		return *this;
	}


	bool Socket::bind(Address local) {
		if(sock_fd == -1)  return false;
		if(0 == ::bind(sock_fd, local.generic(), local.generic_size())) {
			bound_addr = std::move(local);
			return true;
		} else {
			return false;
		}
	}


	bool Socket::connect(Address rem) {
		if(sock_fd == -1)  return false;
		if(-1 != ::connect(sock_fd, rem.generic(), rem.generic_size())) {
			connected_addr = std::move(rem);
			return true;
		} else {
			return false;
		}
	}

	bool Socket::listen(unsigned int backlog) {
		if(sock_fd == -1)  return false;
		return -1 != ::listen(sock_fd, backlog);
	}


	Socket Socket::accept() {
		if(sock_fd != -1) {
			sockaddr recv_addr;
			socklen_t recv_addr_size = sizeof(sockaddr);
			::memset(&recv_addr, 0, sizeof(sockaddr));
			int retn_sock = ::accept(sock_fd, &recv_addr, &recv_addr_size);
			if(retn_sock == -1)  return Socket(-1);
			connected_addr = Address(recv_addr, recv_addr_size, type(), protocol());
			return Socket(
					retn_sock,
					boundAddress(),
					Address(
						recv_addr,  recv_addr_size,
						bound_addr.socketType(),
						bound_addr.protocol()
					) );
		}
		return Socket(-1);
	}


	void Socket::close() { this->~Socket(); }


	ssize_t Socket::read(void* dest, size_t max, unsigned int flags) {
		if(sock_fd == -1)  return -1;
		return ::recv(sock_fd, dest, max, flags | MSG_NOSIGNAL);
	}

	ssize_t Socket::write(const void * src, size_t size, unsigned int flags) {
		if(sock_fd == -1)  return -1;
		return ::send(sock_fd, src, size, flags | MSG_NOSIGNAL);
	}

}
