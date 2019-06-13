#include <csono/args.hpp>



namespace {

	/* The array was created using raw new(...) and placement
	 * new(...)s, so technically each element has to be deleted
	 * individually and not by delete[]ing the base pointer. */
	void delete_all_args(Argument* args, unsigned size) {
		for(unsigned i=0; i < size; ++i)
			args[i].~Argument();
		::operator delete(args);
	}

}



namespace csono {

	ArgsParser::ArgsParser():
			ap_args(nullptr),
			ap_size(0), ap_offset(0)
	{ }


	ArgsParser::ArgsParser(const ArgsParser & cpy):
			ap_args((Argument*) ::operator new(cpy.ap_size * sizeof(Argument))),
			ap_size(cpy.ap_size), ap_offset(cpy.ap_offset)
	{
		for(unsigned i=0; i < ap_size; ++i)
			new (ap_args + i) Argument(ap_args[i]);
	}


	ArgsParser::ArgsParser(ArgsParser&& mov):
			ap_args(std::move(mov.ap_args)),
			ap_size(std::move(mov.ap_size)),
			ap_offset(std::move(mov.ap_offset))
	{
		mov.ap_args = nullptr;  mov.ap_size = 0;  mov.ap_offset = 0;
	}


	ArgsParser::ArgsParser(const char ** args, int n):
			ap_offset(0)
	{
		// Calculate size, if unknown
		if(n < 0) {
			n = 0;
			while(args[n] != nullptr)  ++n;
		}
		ap_size = n;

		// Allocate array, then initialize each element
		ap_args = (Argument*) ::operator new(n * sizeof(Argument));
		for(int i=0; i<n; ++i)
			new (ap_args + i) Argument(args[i], i);
	}


	ArgsParser::~ArgsParser() {
		if(ap_args != nullptr)  delete_all_args(ap_args, ap_size);
	}


	ArgsParser& ArgsParser::operator = (const ArgsParser & cpy) {
		// ... cowboy
		this->~ArgsParser();
		new (this) ArgsParser(cpy);
		return *this;
	}


	ArgsParser& ArgsParser::operator = (ArgsParser&& mov) {
		this->~ArgsParser();
		new (this) ArgsParser(std::move(mov));
		return *this;
	}


	Argument ArgsParser::operator [] (unsigned i) const {
		if(i < ap_size)  return ap_args[i];
		else  return Argument();
	}

	const ArgsParser & ArgsParser::shift(int delta) {
		delta = ((int) ap_offset) + delta;
		if(delta < 0) {
			ap_offset = 0;
		} else
		if(((unsigned) delta) > ap_size) {
			ap_offset = ap_size;
		} else {
			ap_offset = delta;
		}
		return *this;
	}

}
