#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

void thread_code0(void) {
	std::cout << "Hi." << std::endl;
}

static const int num_threads = 8;

int main(int argc, char** argv) {
	const unsigned int hw = std::thread::hardware_concurrency();


	std::vector<std::thread> threads;
	threads.resize(num_threads);
	for (int i = 0; i < num_threads; ++i) {
		threads[i] = std::thread(thread_code0, i);
	}
	for (int i = 0; i < 8; ++i) {
		threads[i].join();
	}

	// Do I care, what data type it is? NO! Auto-deduce type...
	auto hw2 = std::thread::hardware_concurrency();
	std::cout << "Counting threads...\n"; // End-Of-Line without flush (faster)
	std::this_thread::sleep_for(std::chrono::milliseconds(25));
	std::this_thread::sleep_for(std::chrono::seconds(3));
	std::cout << "Got HW threads: " << hw << std::endl; // End-Of-Line with implicit flush (safer, slower)
	return EXIT_SUCCESS;
}