#include <barrier>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
 
int main()
{
    const auto workers = {"Anil", "Busara", "Carl"};
    std::cout << "main thread " << std::this_thread::get_id() << std::endl;
    // 当所有的线程都到达了barrier，有且仅有一个线程调用on_completion
    auto on_completion = []() noexcept
    {
        std::cout << std::this_thread::get_id() << std::endl;
        // 不需要加锁
        static auto phase =
            "... done\n"
            "Cleaning up...\n";
        std::cout << phase;
        phase = "... done\n";
    };
 
    std::barrier sync_point(std::ssize(workers), on_completion);
 
    // 线程实现可以看到，barrier是可以复用的
    auto work = [&](std::string name)
    {
        std::string product = "  " + name + " worked\n";
        std::cout << product;
        std::cout << std::this_thread->get_id() << std::endl;
        sync_point.arrive_and_wait();
 
        product = "  " + name + " cleaned\n";
        std::cout << product;
        sync_point.arrive_and_wait();
    };
 
    std::cout << "Starting...\n";
    std::vector<std::thread> threads;
    threads.reserve(std::size(workers));
    for (auto const& worker : workers)
        threads.emplace_back(work, worker);
    for (auto& t : threads)
        t.join();
}