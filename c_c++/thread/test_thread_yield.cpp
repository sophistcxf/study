#include <iostream>
#include <chrono>
#include <thread>
 
void little_sleep(std::chrono::microseconds us)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + us;
    do {
	// 让出CPU时间片
        std::this_thread::yield();
    } while (std::chrono::high_resolution_clock::now() < end);
}
 
int main()
{
    auto start = std::chrono::high_resolution_clock::now();
 
    little_sleep(std::chrono::microseconds(100));
 
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    std::cout << "waited for "
              << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
              << " microseconds\n";
}
