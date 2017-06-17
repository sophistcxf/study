/*************************************************************************
  > File Name: test_mpi.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 22 Jun 2016 05:51:38 PM CST
 ************************************************************************/

/*
 * run with command
 * ../../../mpich/lib/mpich/bin/mpirun -np 4 ./test_mpi
 */
#include <iostream>
using namespace std;

#include <boost/mpi.hpp>

int main(int argc, char *argv[])
{
  boost::mpi::environment env(argc, argv);
  boost::mpi::communicator world;
  std::cout << world.rank() << ", " << world.size() << '\n';
  return 0;
}
