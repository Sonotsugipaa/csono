export CPPFLAGS=-std=c++17 -g -Wall -Wpedantic -I./include -Llib
export ALL_MODS=$(patsubst src/module_%,%,$(wildcard src/module_*))
export ALL_OBJS=$(patsubst %,build/%.o,$(ALL_MODS))
export ALL_EXES=$(patsubst src/%.cpp,bin/%,$(wildcard src/*.cpp))
export LIBS=#no-lib#

# objects should not be removed automatically
.PHONY: build/%.o


all: $(ALL_EXES)

build/%.o: src/module_%/makefile
	make --file=$< $@

build/%.o: src/module_%/compile.cpp
	g++ $(CPPFLAGS) $(OBJFLAGS) -I$(dir $<) -c -o$@ $<

lib/libcsono.a: $(ALL_OBJS)
	ar -rvs $@ $^

bin/%: src/%.cpp lib/libcsono.a $(ALL_OBJS)
	g++ $(CPPFLAGS) -o$@ $< -lcsono $(LIBS)

clean:
	rm -rf bin lib
	mkdir bin lib

reset:
	rm -rf bin build lib
	mkdir bin build lib
