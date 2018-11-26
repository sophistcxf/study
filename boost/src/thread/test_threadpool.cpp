/*************************************************************************
  > File Name: test_threadpool.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Tue 21 Jun 2016 02:57:47 PM CST
 ************************************************************************/
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>


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

void add(int x, int y)
{
}


struct P
{
    boost::thread_group* threadpool;
    int thread_count;
    boost::asio::io_service* io_service;
    boost::asio::io_service::work** work;
};

void start_threadpool(void* param)
{
    P* p = (P*)param;
    *p->work = new boost::asio::io_service::work(*p->io_service);
    for (int i = 0; i < p->thread_count; ++i)
        p->threadpool->create_thread(boost::bind(&boost::asio::io_service::run, p->io_service));
    //p->threadpool->join_all();
}

int main()
{

    /*
     * Create an asio::io_service and a thread_group (through pool in essence)
     */
    boost::asio::io_service io_service;
    boost::thread_group threadpool;


    /*
     * This will start the io_service processing loop. All tasks 
     * assigned with io_service.post() will start executing. 
     */
    boost::asio::io_service::work* work;

    /*
     * This will add 2 threads to the thread pool. (You could just put it in a for loop)
     */
    int thread_count = 10;
    P p;
    p.threadpool = &threadpool;
    p.io_service = &io_service;
    p.thread_count = thread_count;
    p.work = &work;
    //start_threadpool(&p);
    boost::thread t(start_threadpool, &p);

    /*
     * This will assign tasks to the thread pool. 
     * More about boost::bind: "http://www.boost.org/doc/libs/1_54_0/libs/bind/bind.html#with_functions"
     */
    while (true)
    {
        io_service.post(boost::bind(myTask, "Hello World!"));
        io_service.post(boost::bind(clearCache, "./cache"));
        io_service.post(boost::bind(getSocialUpdates, "twitter,gmail,facebook,tumblr,reddit"));
        TwoFactor f(2.5, 10.8);
        io_service.post(boost::bind(add, &f));
        io_service.post(boost::bind(add, 1, 2));
        sleep(5);
    }

    /*
     * This will stop the io_service processing loop. Any tasks
     * you add behind this point will not execute.
     */
    //io_service.stop();
}
