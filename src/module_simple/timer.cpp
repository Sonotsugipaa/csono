#include <csono/timer.hpp>

using namespace std::chrono;



namespace csono {

	Timer::Timer() {
		reset();
	}

	void Timer::reset() {
		time = steady_clock::now();
	}

	float Timer::s() const {
		duration<double, std::ratio<1>> s = steady_clock::now() - time;
		return s.count();
	}

	float Timer::millis() const {
		duration<double, std::milli> millis = steady_clock::now() - time;
		return millis.count();
	}

	float Timer::micros() const {
		duration<double, std::micro> micros = steady_clock::now() - time;
		return micros.count();
	}

	float Timer::nanos() const {
		duration<double, std::micro> nanos = steady_clock::now() - time;
		return nanos.count();
	}

}
