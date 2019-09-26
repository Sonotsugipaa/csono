#include <csono/serial.hpp>



namespace csono {

	Serial::Serial(Signature sig, std::string pl):
			signature(std::move(sig)),
			payload(std::move(pl))
	{ }


	Serial Serial::deserialize(const char * src, size_t src_len) {
		Serial retn;
		if(src[4] == ';') {
			retn.signature = src;
			if(retn.signature) {
				retn.payload = std::string(src+5, src_len-5);
				return retn;
			}
		}
		return Serial();
	}


	std::string Serial::serialize() const {
		return ((std::string) signature) + ';' + payload;
	}

}
