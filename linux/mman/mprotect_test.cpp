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

int main(int argc, char *argv[])
{
	pagesize = sysconf(_SC_PAGE_SIZE);
	if (pagesize == -1)
		handle_error("sysconf");

	printf("page size is %d\n", pagesize);

    int rlt = posix_memalign((void**)&buffer, 8, 4 * pagesize);
    if (rlt != 0) {
		handle_error("memalign");
    }

	if (buffer == NULL)
		handle_error("memalign");

	buffer_beg = (long)buffer;
	buffer_end = (long)buffer + 4*pagesize;

	printf("Start of region:        0x%lx\n", (long) buffer);
	printf("End of region:        0x%lx\n", (long) buffer+4*pagesize);

	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handler;

	if (sigaction(SIGSEGV, &sa, NULL) == -1)
		handle_error("sigaction for SIGSEGV");

    // 使用 mprotect 保护起来的页
    // 当与权限冲突时，Linux只发送SIGSEGV消息，MAC会发送SIGBUS消息
    if (sigaction(SIGBUS, &sa, NULL) == -1)
        handle_error("sigaction for SIGBUS");

	// 将第3页保护起来
	if (mprotect(buffer + pagesize * 2, pagesize,
				PROT_READ) == -1)
		handle_error("mprotect");

	for (char* p = buffer ; ; )
		*(p++) = 'a';

	exit(EXIT_SUCCESS);
}
