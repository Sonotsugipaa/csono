#include <iostream>

#include "testdefs.hpp"

#include <csono/args.hpp>



namespace csono {
	inline namespace test {

		ArgsParser mov(const char ** args) {
			return args;
		}

		bool perform_args(const char ** args) {
			ArgsParser ap = mov(args);
			bool fail = false;
			unsigned i = 1;
			while(ap) {
				Argument arg = ap.current();
				std::cout << "Argument "<<i<<": " << arg.value() << ' ';
				fail = fail && (arg.value() == (std::string("arg")+std::to_string(i)));
				std::cout << "   " << (fail? NO : OK) << '\n';
				++i;  ap.shift();
			}
			return !fail;
		}

		bool perform() {
			const char * args[4] = {"arg1", "arg2", "arg3", nullptr};
			return perform_args(args);
		}

	}
}


int main(int, char**) {
	bool retn = perform();
	std::cout << (retn? SUCCESS : FAILURE) << std::endl;
	return retn;
}
