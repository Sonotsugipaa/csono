HERE=src/module_socket
ALL_OBJS_SOCKET=$(patsubst $(HERE)/%.cpp,build/socket_%.o,$(wildcard $(HERE)/*.cpp))

build/socket_%.o: $(HERE)/%.cpp include/csono/socket.hpp
	g++ $(CPPFLAGS) $(OBJFLAGS) -c -o$@ $<

build/socket.o: $(ALL_OBJS_SOCKET)
	ld -o$@ -r $^
