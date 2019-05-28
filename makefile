export CPPFLAGS=-std=c++17 -g -Wall -I./include -Llib
export ALL_MODS=$(patsubst src/module_%,%,$(wildcard src/module_*))
export ALL_OBJS=$(patsubst %,build/%.o,$(ALL_MODS))
export ALL_EXES=$(patsubst src/%.cpp,bin/%,$(wildcard src/*.cpp))
export LIBS=#no-lib#

# objects should not be removed automatically
.PRECIOUS: build/%.o


all: $(ALL_EXES)

build/%.o: src/module_%/makefile
	make --file=$< $@

build/%.o: src/module_%/compile.cpp
	g++ $(CPPFLAGS) -I$(dir $<) -c -o$@ $<

bin/%: src/%.cpp $(ALL_OBJS)
	g++ $(CPPFLAGS) -o$@ $^ $(LIBS)

reset:
	rm -rf bin build
	mkdir bin build
