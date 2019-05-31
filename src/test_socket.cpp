#include <csono/socket.hpp>

#include <iostream>
#include <thread>
#include <chrono>



namespace csono::test {

	void sleep_ms(unsigned int ms) {
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}


	uint16_t try_bind(
			Socket& socket,
			uint16_t port = 10000,
			uint16_t retries = 10
	) {
		std::cout << "Trying to bind socket fd#" << socket.fd() << " on port " << port << "..." << std::endl;
		while(! socket.bind(port)) {
			if(retries == 0) {
				std::cerr << "Failed to find a bindable port" << std::endl;
				return 0;
			}
			sleep_ms(500);
			++port;  --retries;
			std::cout << "Trying to bind socket fd#" << socket.fd() << " on port " << port << "..." << std::endl;
		}
		return port;
	}


	bool perform_udp(uint16_t port = 10000) {
		Socket socket_in = Socket::Udp(AF_INET);
		Socket socket_out = Socket::Udp(AF_INET);
		char recv[4];  recv[0]='o';  recv[1]='l';  recv[2]='o';

		if(socket_in && socket_out) {
			port = try_bind(socket_in, port);
			socket_out.connect(Address::v4("127.0.0.1", port));
			socket_out.write("lol", 3);
			socket_in.read(recv, 3);  recv[3] = '\0';
		}

		bool retn = (std::string(recv) == "lol");
		std::cout << "UDP: " << (retn? "ok\n" : "no\n");
		return retn;
	}


	bool perform_tcp(uint16_t port = 11000) {
		Socket socket_in = Socket::Tcp(AF_INET);
		Socket socket_out = Socket::Tcp(AF_INET);
		char recv[4];  recv[0]='o';  recv[1]='l';  recv[2]='o';

		if(socket_in && socket_out) {
			port = try_bind(socket_in, port);
			socket_in.listen(4);
			socket_out.connect(Address::v4("127.0.0.1", port));
			Connection conn = socket_in.accept();
			socket_out.write("lol", 3);
			conn.socket().read(recv, 3);  recv[3] = '\0';
		}

		bool retn = (std::string(recv) == "lol");
		std::cout << "TCP: " << (retn? "ok\n" : "no\n");
		return retn;
	}


	bool perform() {
		return perform_udp() && perform_tcp();
	}

}


int main(int, char**) {
	std::cout << " ----- SOCKET TEST -----" << std::endl;
	bool result = csono::test::perform();
	std::cout << "\nResult: " << (result? "success" : "failure") << std::endl;
	return EXIT_SUCCESS;
}
