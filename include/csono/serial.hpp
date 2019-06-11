#ifndef CSONO_SERIAL_HPP
#define CSONO_SERIAL_HPP

#include <csono/signature.hpp>



inline namespace csono {

	class Serial {
	public:
		Signature signature;
		std::string payload;

		Serial(Signature = Signature(), std::string = "");

		static Serial deserialize(const char * buffer, size_t buffer_size);

		std::string serialize() const;
		inline operator std::string() const { return serialize(); }

		constexpr operator bool () const { return signature != 0; }
	};

}

#endif
