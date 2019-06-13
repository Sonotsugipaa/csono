export CPPFLAGS=-std=c++17 -g -Wall -Wpedantic -I./include -Llib
export ALL_MODS=$(patsubst src/module_%,%,$(wildcard src/module_*))
export ALL_OBJS=$(patsubst %,build/%.o,$(ALL_MODS))
export ALL_EXES=$(patsubst src/%.cpp,bin/%,$(wildcard src/*.cpp))

# objects and libraries should not be removed automatically
.SECONDARY:

.PHONY: install uninstall


all: $(ALL_EXES)
	make install

build/%.o: src/module_%/makefile
	make --file=$< $@

build/%.o: src/module_%/compile.cpp
	g++ $(CPPFLAGS) $(OBJFLAGS) -I$(dir $<) -c -o$@ $<

lib/libcsono.a: $(ALL_OBJS)
	ar -rvs $@ $^

bin/%: src/%.cpp lib/libcsono.a $(ALL_OBJS)
	g++ $(CPPFLAGS) -o$@ $< -lcsono $(LIBS)

install: lib/libcsono.a
	# --------------------------------------#
	# Installing the library in /usr/local/ #
	# --------------------------------------#
	mkdir -p /usr/local/include/csono
	cp -rf include/csono/. /usr/local/include/csono/.
	cp -f lib/libcsono.a /usr/local/lib/libcsono.a

uninstall:
	# ------------------------------------#
	# Removing the library in /usr/local/ #
	# ------------------------------------#
	rm -r /usr/local/include/csono
	rm /usr/local/lib/libcsono.a

clean:
	rm -rf bin lib
	mkdir bin lib

reset:
	rm -rf bin build lib
	mkdir bin build lib
