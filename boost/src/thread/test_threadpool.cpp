/*************************************************************************
  > File Name: test_threadpool.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Tue 21 Jun 2016 02:57:47 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include <iostream>
#include <boost/threadpool.hpp>
#include <boost/thread/thread.hpp>

using namespace boost::threadpool;

class Task{

  private:
    std::string user;

  public:
    Task(){}
    ~Task(){}

    Task(std::string input):user(input){}

    void operator()(){
      std::cout<<"Hello "<<user<<std::<span class="hiddenSpellError">endl;
      sleep(5);
      delete this;/*Trust me its safe */

    }
};

int main(int argc, char** argv)
{
  /*Create two threads */
  pool Thread_Pool(2);

  int count;

  if(argc > 1)
  {
    for (count = 1; count < argc; count++)
    {

      Task *Thread_Object = new Task(argv[count]); /*Create an object of class Task and pass names*/
      Thread_Pool.schedule(boost::ref(*Thread_Object));/*Schedule all threads at once */

    }
  }
  else{

    std::cout<<"Give some names Please  "<<std::<span class="hiddenSpellError">endl;
  }

  std::cout<<"All Threads scheduled,now wait for their execution"<<std::<span class="hiddenSpellError">endl;

  return 0;
}
