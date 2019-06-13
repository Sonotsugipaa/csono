#include <csono/args.hpp>



namespace csono {

	Argument::Argument(): a_value(), a_position(-1) { }

	Argument::Argument(std::string val, unsigned pos):
			a_value(std::move(val)),
			a_position(pos)
	{ }

}
