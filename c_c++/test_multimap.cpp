/*************************************************************************
  > File Name: test_multimap.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Mon 05 Jun 2017 11:47:44 PM CST
 ************************************************************************/

#include <iostream>
#include <map>
using namespace std;

int main()
{
    multimap<unsigned, unsigned> m;
    m.insert(make_pair(0, 10));
    m.insert(make_pair(0, 20));
    for (multimap<unsigned, unsigned>::iterator it = m.begin();
            it != m.end(); ++it)
    {
        cout << it->second << endl;
    }
    return 0;
}
