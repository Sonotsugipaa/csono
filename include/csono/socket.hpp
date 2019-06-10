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
	class Listener;


	class Address {
		friend Socket;

	public:
		struct Node {
			int flags;
			int family;
			int socket_type;
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

		/* Only meant to be used by Socket::accept() and Socket::read(...);
		 * the constructor makes a hard copy of the sockaddr, so
		 * the first argument may point to temporary data. */
		Address(const sockaddr &, socklen_t, int socktype, int protocol);

	public:
		Address();
		Address(Node);
		Address(const char * host, const char * service);

		inline Address(const char * host, uint16_t port):
				Address::Address(host, std::to_string(port).c_str())
		{ }

		constexpr int family() const { return nodes.ptr->family; }
		constexpr int socketType() const { return nodes.ptr->socket_type; }
		constexpr int protocol() const { return nodes.ptr->protocol; }

		inline const std::string & hostname() const { return nodes.ptr->name; }
		uint16_t port() const;
		inline const std::string & fullname() const { return nodes.ptr->fullname; }
		inline operator std::string () const { return fullname(); }

		constexpr operator bool () const { return nodes.size != 0; }
		constexpr bool operator ! () const { return nodes.size == 0; }

		constexpr sockaddr* generic() { return nodes.ptr->addr; }
		constexpr const sockaddr * generic() const { return nodes.ptr->addr; }
		constexpr socklen_t generic_size() const { return nodes.ptr->addr_len; }

		inline Address operator [] (unsigned i) const {
			if(i > nodes.size)  return Address();
			return Address(nodes.ptr[i]);
		};
		constexpr unsigned size() const { return nodes.size; }

		constexpr bool operator == (const Address & r) {
			return
					(socketType() == r.socketType()) &&
					(fullname() == r.fullname());
		}
		constexpr bool operator != (const Address & r) { return ! (*this == r); }

	};


	class Socket {
	private:
		int sock_fd = -1;
		int sock_type;
		int sock_protocol;
		Address bound_addr;
		Address connected_addr;

		explicit Socket(int fd, Address local = Address(), Address remote = Address());

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

		/* Default constructor creates an invalid socket;
		 * use the specialized UDP or TCP classes */
		Socket(): Socket(-1) { }

		explicit Socket(int addr_family, int socket_type, int protocol = 0);
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
		constexpr int type() const { return sock_type; }
		constexpr int protocol() const { return sock_protocol; }
		constexpr const Address & boundAddress() const { return bound_addr; }
		constexpr const Address & connectedAddress() const { return connected_addr; }

		Socket accept();

		void close();

		ssize_t  read(void* dest, size_t max, unsigned int flags = 0);
		ssize_t write(const void * src, size_t size, unsigned int flags = 0);

		ssize_t  read(Address& remote, void* dest, size_t max, unsigned int flags = 0);
		ssize_t write(Address remote, const void * src, size_t size, unsigned int flags = 0);
	};


	class Socket::Tcp : public Socket {
	public:
		Tcp(int addr_family = AF_UNSPEC):
				Socket::Socket(addr_family, SOCK_STREAM, IPPROTO_TCP)
		{ }
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
		Udp(int addr_family = AF_UNSPEC):
				Socket::Socket(addr_family, SOCK_DGRAM, IPPROTO_UDP)
		{ }
	};
	class Socket::Udp4 : public Socket {
	public:
		Udp4(): Socket::Socket(AF_INET,  SOCK_DGRAM,  IPPROTO_UDP) { }
	};
	class Socket::Udp6 : public Socket {
	public:
		Udp6(): Socket::Socket(AF_INET6, SOCK_DGRAM,  IPPROTO_UDP) { }
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
		Listener& operator = (Listener&&) = default;

		Socket accept();
	};

}

#endif
