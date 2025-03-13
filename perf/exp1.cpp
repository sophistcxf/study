#include <iostream>
#include <vector>
#include <thread>


void foo1() {
	int sum = 0;
	for (int i = 0; i < 1024 * 1024 * 1024; i++) {
		sum += 1;
	}
	std::cout << sum << std::endl;
}
void foo() {
	int sum = 0;
	for (int i = 0; i < 1024 * 1024 * 1024; i++) {
		sum += 1;
	}
	std::cout << sum << std::endl;
	foo1();
}
void foo3() {
	int sum = 0;
	for (int i = 0; i < 1024 * 1024 * 1024; i++) {
		sum += 1;
	}
	std::cout << sum << std::endl;
	foo1();
}

void thread_task(int id) {
	std::cout << "Thread " << id << " is running." << std::endl;
	foo();
}


void thread_task2(int id) {
foo3();
}

int main() {
	const int num_threads = 10; // 线程数量
	std::vector<std::thread> threads; // 存储线程的向量

	for (int i = 0; i < num_threads - 5; ++i) {
		threads.emplace_back(thread_task, i);
	}
	for (int i = num_threads - 5; i < num_threads; ++i) {
		threads.emplace_back(thread_task2, i);
}
	for (auto& t : threads) {
		t.join();
	}
	std::cout << "All threads have finished." << std::endl;

	return 0;
}
