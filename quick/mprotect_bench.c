#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#define PAGE_SIZE 0x1000

int main(int argc, char *argv[])
{
	int i, r;
	void *p;

	p = mmap(NULL, PAGE_SIZE, PROT_READ,
		 MAP_ANONYMOUS | MAP_PRIVATE | MAP_POPULATE, -1, 0);

	if (p == MAP_FAILED) {
		printf("mmap failed!\n");
		return -1;
	}

	printf("mmap = %p\n", p);


	for (i = 0; i < 1000000; i++) {
		r = mprotect(p, PAGE_SIZE, PROT_READ | PROT_WRITE);
		if (r)
			break;

		r = mprotect(p, PAGE_SIZE, PROT_READ); 
		if (r)
			break;
	}

	if (r)
		printf("mprotect failed!\n");

	return 0;
}
