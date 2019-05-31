#include <csono/socket.hpp>

#include <iostream>



namespace csono::test {

	bool perform() {
		Socket socket_in = Socket(AF_INET, SOCK_DGRAM);
		Socket socket_out = Socket(AF_INET, SOCK_DGRAM);
		uint16_t port = 10000;
		char recv[4];  recv[0]='l';  recv[1]='l';  recv[2]='l';

		std::cout << "Trying to bind on port " << port << "..." << std::endl;
		while(! socket_in.bind(port)) {
			++port;
			std::cout << "Trying to bind on port " << port << "..." << std::endl;
		}

		socket_out.connect(Address::v4("127.0.0.1", port));
		socket_out.write("lol", 4);
		socket_in.read(recv, 4);  recv[3] = '\0';

		return std::string(recv) == "lol";
	}

}


int main(int, char**) {
	bool result = csono::test::perform();
	std::cout << "\nResult: " << (result? "success" : "failure") << std::endl;
	return EXIT_SUCCESS;
}
