/*************************************************************************
  > File Name: main.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sun 23 Jul 2017 10:34:19 PM CST
 ************************************************************************/

#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/sem.h> 

static int sem_id = 0;  

static int set_semvalue();  
static void del_semvalue();  
static int semaphore_p();  
static int semaphore_v(); 


static int set_semvalue()
{
	union semun sem_union;
	sem_union.val = 1;
	if (semctl(sem_id, 1, SETVAL, sem_union) == -1)
		return 0;
	return 1;
}

static int get_semvalue()
{
    return semctl(sem_id, 1, GETVAL);
}

static void del_semvalue()
{
	// 删除信号量
	union semun sem_union;
	if (semctl(sem_id, 1, IPC_RMID, sem_union) == -1)
		fprintf(stderr, "Failed to delete semaphore\n");
}

static int semaphore_p()
{
	// 对信号
	struct sembuf sem_b;
	sem_b.sem_num = 1;
	sem_b.sem_op = -1;
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1)
	{
		fprintf(stderr, "semaphore_p failed\n");
		return 0;
	}
	return 1;
}

static int semaphore_v()
{
	struct sembuf sem_b;
	sem_b.sem_num = 1;
	sem_b.sem_op = 1;
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id, &sem_b, 1) == -1)
	{
		fprintf(stderr, "semaphore_v failed\n");
		return 0;
	}
	return 1;
}

int main(int argc, char* argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);
	sem_id = semget((key_t)1234, 2, 0666 | IPC_CREAT);
    char message = 'X';
    if (sem_id == -1)
    {
        fprintf(stderr, "Failed semget\n");
        exit(1);
    }
	if (argc > 1)
	{
		if (!set_semvalue())
		{
			fprintf(stderr, "Failed to initialize semaphore\n");
			exit(EXIT_FAILURE);
		}
		message = argv[1][0];
		sleep(2);
	}
	for (int i = 0; i < 10; ++i)
	{
        if (argc > 1)
        {
            if (!semaphore_p())
                exit(EXIT_FAILURE);
        }
        else
        {
            if (!semaphore_v())
                exit(EXIT_FAILURE);
        }
        printf("sema value: %d\n", get_semvalue());
		printf("%c\n", message);
		sleep(1);
	}
	printf("\n%d - finished\n", getpid());
	if (argc > 1)
	{
		sleep(3);
		del_semvalue();
	}
	exit(EXIT_FAILURE);
}
