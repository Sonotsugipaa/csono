#ifndef CSONO_SOCKET_HPP
#define CSONO_SOCKET_HPP

extern "C" {
	#include <netdb.h>
	#include <unistd.h>

	#include <arpa/inet.h>

	#include <sys/types.h>
	#include <sys/socket.h>
}

#include <cstdint>

#include <string>

#include <csono/identity.tpp>

#define SOCKET_BACKLOG_DEFAULT  8



inline namespace csono {

	class Socket;
	class Connection;
	class Listener;


	class Address {
		friend Connection;
		friend Socket;
	public:
		Address();

		union {
			//sockaddr_un  unix;
			sockaddr_in  v4;
			sockaddr_in6 v6;
		} value;
		unsigned int size;

		static Address unix(std::string host, uint16_t port);
		static Address v4(std::string host, uint16_t port);
		static Address v6(std::string host, uint16_t port);

		constexpr sockaddr* generic() { return reinterpret_cast<sockaddr*>(&value); }
		constexpr const sockaddr * generic() const { return reinterpret_cast<const sockaddr *>(&value); }
	};


	class Socket {
		friend Connection;
	private:
		int sock_fd = -1;

		Socket(int fd);

	public:
		class Tcp;
		class Udp;

		Socket(int addr_family, int socket_type, int protocol = 0);
		Socket(const Socket &) = delete;
		Socket(Socket&&);
		~Socket();

		Socket& operator = (const Socket &) = delete;
		Socket& operator = (Socket&&);

		constexpr operator bool () const { return sock_fd != -1; }
		constexpr bool operator ! () const { return sock_fd == -1; }

		bool bind(uint16_t port);

		bool listen(unsigned int backlog);
		bool connect(Address remote_host);

		constexpr int fd() const { return sock_fd; }

		Connection accept();

		void close();

		ssize_t  read(void* dest, size_t max, unsigned int flags = 0);
		ssize_t write(const void * src, size_t size, unsigned int flags = 0);
	};


	class Socket::Tcp : public Socket {
	public:
		Tcp(int addr_family);
	};

	class Socket::Udp : public Socket {
	public:
		Udp(int addr_family);
	};


	class Connection {
		friend Listener;
		friend Socket;
	public:
		using unique_t = uid::Uid<Connection>;
		using uid_t = unique_t::key_t;

	private:
		Address _address;
		Socket _remote_socket;
		unique_t _unique;

		Connection(Address address, int socket_fd);

	public:
		Connection();
		Connection(const Connection &) = delete;
		Connection(Connection&&) = default;
		constexpr operator bool () const   { return   _remote_socket; }
		constexpr bool operator ! () const { return ! _remote_socket; }
		inline Address address() const { return _address; }
		inline Socket& socket() { return _remote_socket; }
		inline const Socket & socket() const { return _remote_socket; }
		inline uid_t uid() const { return _unique.uid(); }
	};


	class Listener {
	private:
		Socket passive_socket;
		uint16_t backlog;

	public:
		Listener(Socket&& socket, uint16_t port, unsigned int backlog = SOCKET_BACKLOG_DEFAULT);
		Listener(const Listener &) = delete;
		Listener(Listener&&) = default;

		constexpr operator bool () const   { return   passive_socket; }
		constexpr bool operator ! () const { return ! passive_socket; }

		Listener& operator = (const Listener &) = delete;
		Listener& operator = (Listener&&);

		Connection accept();
	};

}

#endif
