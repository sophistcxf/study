/*************************************************************************
  > File Name: test_threadpool.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Tue 21 Jun 2016 02:57:47 PM CST
 ************************************************************************/
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>


void myTask(const void* p)
{
    char* c = (char*)p;
    std::cout << c << std::endl;
}

void clearCache(const void*p)
{
    char* c = (char*)p;
    std::cout << "clearCache " << c << std::endl;
}

void getSocialUpdates(const void* p)
{
    char* c = (char*)p;
    std::cout << c << std::endl;
}

struct TwoFactor
{
    TwoFactor(double x, double y) : f1(x), f2(y) {}
    double f1;
    double f2;
};

void add(const void* p)
{
    TwoFactor* f = (TwoFactor*)p;
    double sum = f->f1 + f->f2;
    std::cout << f->f1 << "+" << f->f2 << "=" << sum << std::endl;
}

int main()
{
    /*
     * Create an asio::io_service and a thread_group (through pool in essence)
     */
    boost::asio::io_service ioService;
    boost::thread_group threadpool;


    /*
     * This will start the ioService processing loop. All tasks 
     * assigned with ioService.post() will start executing. 
     */
    boost::asio::io_service::work work(ioService);

    /*
     * This will add 2 threads to the thread pool. (You could just put it in a for loop)
     */
     int thread_count = 10;
     for (size_t i = 0; i < thread_count; ++i)
        threadpool.create_thread(
                boost::bind(&boost::asio::io_service::run, &ioService)
                );

    /*
     * This will assign tasks to the thread pool. 
     * More about boost::bind: "http://www.boost.org/doc/libs/1_54_0/libs/bind/bind.html#with_functions"
     */
    ioService.post(boost::bind(myTask, "Hello World!"));
    ioService.post(boost::bind(clearCache, "./cache"));
    ioService.post(boost::bind(getSocialUpdates, "twitter,gmail,facebook,tumblr,reddit"));
    TwoFactor f(2.5, 10.8);
    ioService.post(boost::bind(add, &f));

    /*
     * This will stop the ioService processing loop. Any tasks
     * you add behind this point will not execute.
     */
    //ioService.stop();

    /*
     * Will wait till all the threads in the thread pool are finished with 
     * their assigned tasks and 'join' them. Just assume the threads inside
     * the threadpool will be destroyed by this method.
     */
    threadpool.join_all();
}
