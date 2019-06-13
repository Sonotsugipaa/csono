#ifndef CSONO_ARGS_HPP
#define CSONO_ARGS_HPP

#include <string>



inline namespace csono {

	class Argument {
		std::string a_value;
		int a_position;

	public:
		Argument();
		Argument(std::string value, unsigned position);

		inline const std::string & value() const { return a_value; }
		inline int position() const { return a_position; }

		inline operator const char * () const { return a_value.c_str(); }
		inline operator const std::string & () const { return a_value; }

		inline operator bool   () const { return a_position >= 0; }
		inline bool operator ! () const { return a_position <  0; }
	};


	class ArgsParser {
	private:
		Argument* ap_args;
		unsigned ap_size;
		unsigned ap_offset;

	public:
		ArgsParser();
		ArgsParser(const ArgsParser &);
		ArgsParser(ArgsParser&&);
		ArgsParser(const char ** args, int number = -1);
		~ArgsParser();

		ArgsParser& operator = (const ArgsParser &);
		ArgsParser& operator = (ArgsParser&&);

		Argument operator [] (unsigned index) const;

		inline operator Argument () const { return (*this)[ap_offset]; }
		inline Argument current() const { return static_cast<Argument>(*this); }
		const ArgsParser & shift(int delta = 1);
		inline ArgsParser & reset() { ap_offset = 0;  return *this; }

		inline operator bool   () const { return ap_offset <  ap_size; }
		inline bool operator ! () const { return ap_offset >= ap_size; }
	};

}

#endif
