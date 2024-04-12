/*************************************************************************
  > File Name: test_vector.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Fri 05 Aug 2016 05:57:06 PM CST
 ************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


void test_erase()
{
  vector<int> v;
  for (int i = 0; i < 10; ++i) v.push_back(i);

  for (vector<int>::iterator iter = v.begin(); iter != v.end(); ) {
    if (*iter == 2 || *iter == 5 || *iter == 9)
      iter = v.erase(iter);
    else
      iter++;
  }

  for (int i = 0; i < v.size(); ++i)
    cout << v[i] << endl;
}

bool compare(int n1, int n2)
{
  return n1 < n2;
}

void test_compare()
{
  vector<int> vec;
  vec.push_back(10);
  vec.push_back(3);
  vec.push_back(5);
  vec.push_back(6);
  vec.push_back(10);
  std::stable_sort(vec.begin(), vec.end(), compare);
  for (int i = 0; i < vec.size(); ++i)
    cout << vec[i] << endl;
}

class Message {
public:
    Message() {
        std::cout << "Message::Message()" << std::endl;
    }
    Message(const Message& msg) {
        std::cout << "Message copy constructor" << std::endl;
        type = msg.type;
        value = msg.value;
    }
    char type;
    double value;
};
void test1() {
    std::vector<Message> v;
    v.reserve(10);
    std::cout << v.capacity() << "," << v.size() << std::endl;

    std::vector<Message> v1;
    // 只调用一次无参数的构造函数，10次调用的是拷贝构建函数
    v1.resize(10);

    std::vector<Message> v2;
    v2.resize(10, Message());
}

int main()
{
    test1();
    //test_erase();
}
