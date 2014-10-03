#include <iostream>
#include <savage/clock.hpp>

using namespace savage;

int main(int argc, char* argv[])
{
	savage::clock<std::chrono::microseconds> clock;
	clock.init();
	clock.pause();
	for(int i = 0; i < 10000000; ++i) {}
	clock.resume();
	for(int i = 0; i < 20; ++i) {
		clock.update();
		std::cout << clock.current_time().count() << std::endl;
	}

    return 0;
}
