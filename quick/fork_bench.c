#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#define PAGE_SIZE 0x1000

int main(int argc, char *argv[])
{
	int i, r;
	void *p;

	p = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
		 MAP_ANONYMOUS | MAP_PRIVATE | MAP_POPULATE, -1, 0);

	if (p == MAP_FAILED) {
		printf("mmap failed!\n");
		return -1;
	}

	printf("mmap = %p\n", p);

	for (i = 0; i < 1000000; i++) {
		pid_t pid = fork();

		if (pid == 0)
			exit(0);

		if (pid < 0) {
			printf("fork() %d failed!\n", i);
			return -1;
		}
	}

	return 0;
}
