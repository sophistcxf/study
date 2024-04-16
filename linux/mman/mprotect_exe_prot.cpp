#include <unistd.h>
#include <signal.h>
#include <stdio.h>
//#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <assert.h>

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

char *buffer;
long buffer_beg = 0;
long buffer_end = 0;
int pagesize = 0;

static void handler(int sig, siginfo_t *si, void* unused)
{
	assert(buffer_beg + 2*pagesize == (long)si->si_addr);
	printf("Got SIGSEGV at address: 0x%lx\n", (long) si->si_addr);
	exit(EXIT_FAILURE);
}

static void hello() {
    fprintf(stderr, "hello world\n");
}

int main(int argc, char *argv[])
{
	pagesize = sysconf(_SC_PAGE_SIZE);
	if (pagesize == -1)
		handle_error("sysconf");

	fprintf(stderr, "page size is %d\n", pagesize);

    hello();

    long func_addr = (long)hello;
    long align_func_addr = func_addr / pagesize * pagesize;
    fprintf(stderr, "function hello's address 0x%lx, aligned address 0x%lx\n", func_addr, align_func_addr);

	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handler;

	if (sigaction(SIGSEGV, &sa, NULL) == -1)
		handle_error("sigaction for SIGSEGV");

    if (sigaction(SIGBUS, &sa, NULL) == -1)
        handle_error("sigaction for SIGBUS");

    // 这里会崩溃，why？
	if (mprotect((void*)align_func_addr, pagesize, PROT_NONE) == -1)
		handle_error("mprotect");
    fprintf(stderr, "mprotect successed\n");

    hello();

	exit(EXIT_SUCCESS);
}
