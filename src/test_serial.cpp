#include <iostream>
#include <thread>
#include <chrono>

#include <csono/serial.hpp>

#define OK "\033[1;94mOK\033[m"
#define NO "\033[1;91mNO\033[m"



namespace csono::test {

	bool perform_sig(Signature sig) {
		uint32_t i = sig;
		std::string s = sig;
		std::cout << i << "=\"" << s << "\"    ";
		Signature from_i = i;
		Signature from_s = s.c_str();
		bool retn =
				  (sig == from_s)  &&   (sig == from_i) &&
				(!(sig != from_s)) && (!(sig != from_i));
		std::cout << (retn? OK"\n" : NO"\n");
		return retn;
	}

	bool perform_sig() {
		std::cout << "   --- SIGNATURE ---\n";
		return
				perform_sig(Signature()) &
				perform_sig(Signature("null")) &
				perform_sig(Signature(0x37000000));
	}

	bool perform_serial(Serial serial, std::string should_be) {
		Serial deserial = Serial::deserialize(should_be.c_str(), should_be.size());
		std::string serial_str = serial.serialize();
		std::cout << serial_str << "  ";
		bool retn;
		if(serial)
			retn = (serial_str == should_be) && (serial_str == deserial.serialize());
		else
			retn = (should_be == "");
		std::cout << (retn? OK"\n" : NO"\n");
		return retn;
	}

	bool perform_serial() {
		std::cout << "   --- SERIAL ---\n";
		return
				perform_serial(Serial("baad", "bad request"), "baad;bad request") &
				perform_serial(Serial("ping", ""), "ping;") &
				perform_serial(Serial::deserialize("desr;",5), "desr;") &
				perform_serial(Serial::deserialize("invalid;",8), "");
	}

	bool perform() {
		bool sig = perform_sig();
		std::cout << '\n';
		return sig & perform_serial();
	}

}


int main(int, char**) {
	std::cout << " ----- SERIAL TEST -----" << std::endl;
	bool result = csono::test::perform();
	std::cout << "\nResult: " << (result? "success" : "failure") << std::endl;
	return result? EXIT_SUCCESS : EXIT_FAILURE;
}
