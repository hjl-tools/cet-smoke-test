#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>

#define PAGE_SIZE 0x1000

int main(int argc, char *argv[])
{
	struct rlimit rlim;
	unsigned long ssp;
	unsigned long ssp_len;
	int r, i;

	r = getrlimit(RLIMIT_STACK, &rlim);
	if (r) {
		printf("getrlimit failed.\n");
		return -1;
	}

	ssp_len = rlim.rlim_cur;

	asm volatile("RDSSPQ %0\n": "=r" (ssp));
	printf("ssp = %012lx\n", ssp);
	ssp |= (PAGE_SIZE - 1);
	ssp -= (ssp_len - 1);

	printf("MADV_WILLNEED: %012lx, len = %012lx\n", ssp, ssp_len);
	r = madvise((void *)ssp, ssp_len, MADV_WILLNEED);
	if (r) {
		printf("MADV_WILLNEED failed.\n");
		return -1;
	}

	for (i = 0; i < 10000; i++) {
		pid_t pid = fork();

		if (pid == 0)
			exit(0);

		if (pid < 0) {
			printf("fork() %d failed!\n", i);
			if (i < 10000 || errno != EAGAIN)
				return -1;
			break;
		}
	}

	return 0;
}
