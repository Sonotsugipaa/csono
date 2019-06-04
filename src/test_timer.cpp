#include <csono/timer.hpp>

#include <iostream>
#include <thread>



namespace csono::test {

	void perform() {
		Timer timer;

		std::cout << "// ---- Timer ---------------- //\nSleeping for 1.5s..."
		          << std::flush;
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		std::cout << " Time measured: " << timer.millis() << "ms" << std::endl;

		Timer timer_copy = timer;

		std::cout << "// ---- Timer copy ----------- //\nSleeping for 2.5s..."
		          << std::flush;
		std::this_thread::sleep_for(std::chrono::milliseconds(2500));
		std::cout << " Time measured: " << timer_copy.millis() << "ms" << std::endl;

		timer.reset();

		std::cout << "// ---- Timer after reset ---- //\nSleeping for 1.0s..."
		          << std::flush;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << " Time measured: " << timer.millis() << "ms" << std::endl;
	}

}


int main(int, char**) {
	csono::test::perform();
	std::cout << "\nResult: success... probably" << std::endl;
	return EXIT_SUCCESS;
}
