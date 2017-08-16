/*************************************************************************
  > File Name: test_vector.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Mon 10 Jul 2017 03:27:44 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include <armadillo>

int main()
{
	arma::vec r;
	for (unsigned i = 0; i < 10; ++i)
	{
		r.add(i);
	}
	cout << r.size() << arma::endl;
	cout << arma::var(r) << endl;
	cout << arma::mean(r) << endl;
	return 0;
}
