#include <iostream>
#include <thread>
#include <chrono>

#include <cerrno>

#include "testdefs.hpp"

#include <csono/socket.hpp>



namespace csono::test {

	void sleep_ms(unsigned int ms) {
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}


	Socket mov_socket(Socket&& mov, unsigned recursion = 3) {
		return (recursion > 0)? std::move(mov) : mov_socket(std::move(mov), recursion - 1);
	}


	uint16_t try_bind(
			Socket& socket,
			uint16_t port = 10000,
			uint16_t retries = 10,
			bool v6 = false
	) {
		std::cout << "Trying to bind socket fd#" << socket.fd() << " on port " << port << "..." << std::endl;
		while(! socket.bind(Address(v6? "::" : "0.0.0.0", port))) {
			if(retries == 0) {
				std::cerr << "Failed to find a bindable port" << std::endl;
				return 0;
			}
			sleep_ms(500);
			++port;  --retries;
			std::cout << "Trying to bind socket fd#" << socket.fd() << " on port " << port << "..." << std::endl;
		}
		if(errno == EADDRINUSE)  errno = 0;
		return port;
	}


	void out_cmp_sock(const Socket & socket_i, const Socket & socket_o) {
		std::cout
				<< " >> \""   << socket_i.localAddr().socketType()
				<< ", "       << socket_i.localAddr().fullname()
				<< "\" vs \"" << socket_o.remoteAddr().socketType()
				<< ", "       << socket_o.remoteAddr().fullname() << '"' << std::endl;
	}


	bool perform_udp(uint16_t port = 10000, bool v6 = false) {
		Socket socket_in = Socket::Udp(v6? AF_INET6 : AF_INET);
		Socket socket_out = Socket::Udp(v6? AF_INET6 : AF_INET);
		char recv[4];  recv[0]='o';  recv[1]='l';  recv[2]='o';  recv[3]='\0';

		Address addr_out = Address(v6? "::1" : "127.0.0.1", port);
		Address addr_in_1, addr_in_2;

		if(socket_in && socket_out) {
			port = try_bind(socket_in, port, 5, v6);
			if(port != 0) {
				socket_out.write(addr_out, "lol", 3);
				socket_in.read(addr_in_1, recv, 3);  recv[3] = '\0';
				socket_out.write(addr_out, "lol", 3);
				socket_in.read(addr_in_2, recv, 3);  recv[3] = '\0';
				std::cout
						<< "UDP addresses received: " << addr_in_1.fullname()
						<< ", " << addr_in_2.fullname() << '\n';
			}
		}

		bool retn = (std::string(recv) == "lol");
		if(addr_in_1 != addr_in_2) {
			std::cout << " >> input socket addresses mismatch\n";
			retn = false;
		}
		out_cmp_sock(socket_in, socket_out);
		std::cout << "UDP v" << (v6? '6':'4') << ": " << (retn? OK"\n" : NO"\n");
		return retn;
	}


	bool perform_tcp(uint16_t port = 11000, bool v6 = false) {
		Socket socket_in = Socket::Tcp(v6? AF_INET6 : AF_INET);
		Socket socket_out = mov_socket(Socket::Tcp(v6? AF_INET6 : AF_INET));
		char recv[4];  recv[0]='o';  recv[1]='l';  recv[2]='o';  recv[3]='\0';

		if(socket_in && socket_out) {
			port = try_bind(socket_in, port, 5, v6);
			if(port != 0) {
				socket_in.listen(4);
				if(socket_out.connect(Address(v6? "::1" : "127.0.0.1", port))) {
					Socket conn = mov_socket(socket_in.accept());
					socket_out.write("lol", 3);
					conn.read(recv, 3);  recv[3] = '\0';
				}
			}
		}

		bool retn = (std::string(recv) == "lol");
		if(socket_in.localAddr() != socket_out.remoteAddr()) {
			std::cout << " >> input/output socket addresses mismatch\n";
			retn = false;
		}

		if(errno != 0) {
			std::cout << "ERRNO " << errno << std::endl;
			errno = 0;
			retn = false;
		}

		out_cmp_sock(socket_in, socket_out);
		std::cout << "TCP v" << (v6? '6':'4') << ": " << (retn? OK"\n" : NO"\n");
		return retn;
	}

	std::string addr_out(Address addr) {
		return
				addr.fullname() +
				", fam(" + std::to_string(addr.family()) +
				"), proto(" + std::to_string(addr.protocol()) + ')';
	}

	void perform_addr(const char * host, const char * service) {
		Address result = Address(host, service);
		std::cout << service<<" @ "<<host<<" ("<<result.size()<<" results)\n\t";

		if(result.size() > 0)
			std::cout << ">> "<< addr_out(result[0]);
		for(unsigned i=1; i < result.size(); ++i)
			std::cout << "\n\t>> "<< addr_out(result[i]);

		std::cout << '\n'<<std::endl;
	}

	bool perform_addr_cmp(Address&& addr1, Address&& addr2, bool expect) {
		std::cout
				<< "ADDRESS - comparing "<<addr1.fullname()<<" ("<<addr1.socketType()
				<< ") with "<<addr2.fullname()<<" ("<<addr2.socketType()<<")\n";
		expect = (expect == (addr1 == addr2));
		std::cout << (expect? SUCCESS : FAILURE) << std::endl;
		return expect;
	}

	bool perform_addr_null(Address&& null_addr, bool is_null) {
		bool retn = is_null != (((bool) null_addr) && (!!null_addr));
		if(! retn)
			std::cout
					<< (is_null? "Null address" : "Valid address")
					<< " is " << null_addr.generic()
					<< " (evaluation error)\n";
		return retn;
	}

	bool perform_addr() {
		std::cout << "ADDRESS\n";
		perform_addr("127.0.0.1",   "11000");
		perform_addr("192.169.1.3", "1");
		perform_addr("invalid",     "http");
		perform_addr("::1",         "http");
		return
				perform_addr_null(Address(), true) &
				perform_addr_null(Address("127.0.0.1","http"), false) &
				perform_addr_cmp(Address("127.0.0.1","http"), Address("::1","http"), false) &
				perform_addr_cmp(Address("127.0.0.1","4"), Address("127.0.0.1","4"), true);
	}


	bool perform() {
		return
				perform_addr() &&
				perform_udp(10000, false) & perform_tcp(11000, false) &
				perform_udp(12000, true)  & perform_tcp(13000, true);
	}

}


int main(int, char**) {
	std::cout << " ----- SOCKET TEST -----" << std::endl;
	bool result = csono::test::perform();
	std::cout << "\nResult: " << (result? SUCCESS : FAILURE) << std::endl;
	return EXIT_SUCCESS;
}
