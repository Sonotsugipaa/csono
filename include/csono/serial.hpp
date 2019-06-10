#ifndef CSONO_SERIAL_HPP
#define CSONO_SERIAL_HPP

#include <cstdint>
#include <string>

#define SIGNATURE_PREFIX_LENGTH 5



inline namespace csono {

	class Signature {
	private:
		uint32_t value;

	public:
		constexpr Signature(uint32_t value = 0): value(value) { }
		constexpr Signature(int value): Signature((uint32_t) value) { }

		constexpr Signature(const char * cstr):
				value(0)
		{
			unsigned i = 0;
			while((i<4) && (*cstr != '\0')) {
				value |= (*cstr << static_cast<uint32_t>(24-(8*i)));
				++ cstr;
				++ i;
			}
		}

		inline Signature(const std::string & str): Signature(str.c_str()) { }

		constexpr operator uint32_t() const { return value; }

		inline operator std::string() const {
			std::string str;  str.resize(4);
			str[0] = (value >> (8*3)) & 0xFF;
			str[1] = (value >> (8*2)) & 0xFF;
			str[2] = (value >> (8*1)) & 0xFF;
			str[3] = (value >> (8*0)) & 0xFF;
			return str;
		}

		constexpr bool operator == (Signature r) const { return value == r.value; }
		constexpr bool operator != (Signature r) const { return value != r.value; }
		constexpr bool operator == (int r) const { return value == (uint32_t) r; }
		constexpr bool operator != (int r) const { return value != (uint32_t) r; }
	};


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
