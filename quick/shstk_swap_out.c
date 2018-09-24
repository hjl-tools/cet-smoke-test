#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define SIZE_1_GB 0x40000000
#ifdef __LP64__
#define TOTAL_GBS 20
#else
#define TOTAL_GBS 3
#endif

void *buf[TOTAL_GBS];

void call_to_self(int total)
{
	int count = 0;

	asm volatile("1: cmp %0, %1\n"
		     "jz 2f\n"
		     "inc %0\n"
		     "call 1b\n"
		     "2: cmp $0, %0\n"
		     "jz 3f\n"
		     "dec %0\n"
		     "ret\n"
		     "3:\n"
		     :: "r" (count), "r" (total));
}

int main(int argc, char *argv[])
{
	pid_t child;
	int i, r;
	unsigned char steps;
	void *p;

	child = fork();
	if (child == 0) {
		int n = 10;

		printf("  child pid = %d\n", getpid());

		while (n--) {
			sleep(1);
			call_to_self(0x400000/8);
			printf("  child slept 1 seconds.\n");
		}
		printf("  child done\n");
		exit(0);
	}

	if (child < 0) {
		printf("fork failed.\n");
		return -1;
	}

	for (i = 0; i < TOTAL_GBS; i++) {
		p = malloc(SIZE_1_GB);
		if (!p) {
			printf("malloc failed!\n");
			return -1;
		}
		buf[i] = p;
		memset(p, 0xa5, SIZE_1_GB);
		printf("Used %d GB memory\n", i + 1);
	}

	printf("OK\n");
	return 0;
}

