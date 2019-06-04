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
		struct Node {
			int flags;
			int family;
			int protocol;
			socklen_t addr_len;
			sockaddr* addr;
			std::string name;
			std::string fullname;

			Node();
			Node(const addrinfo &);
			Node(const Node & cpy);
			Node(Node&& mov);
			Node& operator = (const Node & cpy);
			Node& operator = (Node&& mov);
			~Node();
		};

		struct NodeArray {
			Node* ptr;
			unsigned size;

			NodeArray();
			NodeArray(const NodeArray &);
			NodeArray(NodeArray&&);
			NodeArray& operator = (const NodeArray &);
			NodeArray& operator = (NodeArray&&);
			~NodeArray();
		};

	private:
		NodeArray nodes;

	public:
		Address();
		Address(Node);
		Address(const char * host, const char * service);

		inline Address(const char * host, uint16_t port):
				Address::Address(host, std::to_string(port).c_str())
		{ }

		inline int family() const { return nodes.ptr->family; }
		inline int protocol() const { return nodes.ptr->protocol; }

		inline const std::string & hostname() const { return nodes.ptr->name; }
		uint16_t port() const;
		inline const std::string & fullname() const { return nodes.ptr->fullname; }
		inline operator std::string () const { return fullname(); }

		inline operator bool () const { return nodes.ptr != nullptr; }
		inline bool operator ! () const { return nodes.ptr == nullptr; }

		inline sockaddr* generic() { return nodes.ptr->addr; }
		inline const sockaddr * generic() const { return nodes.ptr->addr; }
		inline socklen_t generic_size() const { return nodes.ptr->addr_len; }

		inline Address operator [] (unsigned i) const {
			if(i > nodes.size)  return Address();
			return Address(nodes.ptr[i]);
		};
		constexpr unsigned size() const { return nodes.size; }

	};


	class Socket {
		friend Connection;
	private:
		int sock_fd = -1;

		Socket(int fd);

	public:
		/* Forward declarations for protocol-specific sockets;
		 * they have no overloads, but they remove the need to
		 * specify the address family, the type of socket and the
		 * protocol parameter. */
		class Tcp;
		class Tcp4;
		class Tcp6;
		class Udp;
		class Udp4;
		class Udp6;

		Socket(int addr_family, int socket_type, int protocol = 0);
		Socket(const Socket &) = delete;
		Socket(Socket&&);
		~Socket();

		Socket& operator = (const Socket &) = delete;
		Socket& operator = (Socket&&);

		constexpr operator bool () const { return sock_fd != -1; }
		constexpr bool operator ! () const { return sock_fd == -1; }

		bool bind(Address bind_to);

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
		Tcp(int addr_family): Socket::Socket(addr_family, SOCK_STREAM, IPPROTO_TCP) { }
	};
	class Socket::Tcp4 : public Socket {
	public:
		Tcp4(): Socket::Socket(AF_INET,  SOCK_STREAM, IPPROTO_TCP) { }
	};
	class Socket::Tcp6 : public Socket {
	public:
		Tcp6(): Socket::Socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP) { }
	};

	class Socket::Udp : public Socket {
	public:
		Udp(int addr_family): Socket::Socket(addr_family, SOCK_DGRAM,  IPPROTO_UDP) { }
	};
	class Socket::Udp4 : public Socket {
	public:
		Udp4(): Socket::Socket(AF_INET,  SOCK_DGRAM,  IPPROTO_UDP) { }
	};
	class Socket::Udp6 : public Socket {
	public:
		Udp6(): Socket::Socket(AF_INET6, SOCK_DGRAM,  IPPROTO_UDP) { }
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
		Listener(Socket&& socket, Address bind_to, unsigned int backlog = SOCKET_BACKLOG_DEFAULT);
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
