/*************************************************************************
  > File Name: test_mpich.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 22 Jun 2016 06:06:39 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include "mpich-x86_64/mpi.h"
#include <stdio.h>
#include <math.h>
int main(int argc,char* argv[]){
  int myid, numprocs;
  int namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];

  MPI_Init(&argc,&argv);/* 初始化并行环境 */
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);/* 当前进程的ID号 */
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);/* 进程的总數 */
  MPI_Get_processor_name(processor_name,&namelen);/* 当前处理器的名称 */

  fprintf(stderr,"Hello World! Process %d of %d on %s\n",
      myid, numprocs, processor_name);

  MPI_Finalize();/* 结束并行环境 */
  return 0;
}
