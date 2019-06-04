#include <csono/socket.hpp>



namespace csono {

	Listener::Listener(Socket&& socket, Address bind_to, unsigned int backlog):
			passive_socket(std::move(socket)), backlog(backlog)
	{
		//std::cout << "opening on network-order port " << ::htons(port) << std::endl;
		if(! (passive_socket.bind(std::move(bind_to)) && passive_socket.listen(backlog)))
			passive_socket.close();
	}


	Connection Listener::accept() {
		return passive_socket.accept();
	}

}
