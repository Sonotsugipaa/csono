#ifndef CSONO_TIMER_HPP
#define CSONO_TIMER_HPP

#include <chrono>



inline namespace csono {

	class Timer {
	protected:
		std::chrono::steady_clock::time_point time;

	public:
		Timer();
		Timer(const Timer &) = default;
		Timer(Timer&&) = default;

		Timer& operator = (const Timer &) = default;
		Timer& operator = (Timer&&) = default;

		void reset();

		float s() const;
		float millis() const;
		float micros() const;
		float nanos() const;
	};

}

#endif
