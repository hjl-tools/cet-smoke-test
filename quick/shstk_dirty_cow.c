/*
 * Can we change a shadow stack page?
 */

#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define TEST_PATTERN 0xa5a5a5a5a5a5a5a5UL

void *shstk_buf;
 
void *t1(void *arg)
{
	int i, c = 0;

	for(i = 0; i < 1000000; i++)
		c += madvise(shstk_buf, 0x1000, MADV_DONTNEED);

	printf("t1: madvise return = %d\n",c);
}

void *t2(void *arg)
{
	int f = open("/proc/self/mem", O_RDWR);
	int i, c = 0;
	unsigned long fill = TEST_PATTERN;

	for (i = 0; i < 1000000; i++) {
		lseek(f, (uintptr_t)shstk_buf, SEEK_SET);
		c += write(f, &fill, sizeof(fill));
	}

	printf("t2: write %d bytes\n", c);
}

int child(void)
{
	pthread_t pth1, pth2;

	pthread_create(&pth1, NULL, t1, 0);
	pthread_create(&pth2, NULL, t2, 0);
	pthread_join(pth1, NULL);
	pthread_join(pth2, NULL);
	return 0;
}

int main(int argc, char *argv[])
{
	int stat;
	unsigned long shstk;

	/*
	 * Use the shadow stack page below the
	 * current one; we don't want a #CP.
	 */
	asm volatile("rdsspq %0\n": "=r" (shstk));
	shstk = shstk & 0xfffffffffffff000;
	shstk -= 0x1000;
	shstk_buf = (void *)shstk;

	pid_t child_pid = fork();

	if (child_pid == 0) {
		child();
		exit(0);
	}

	if (child_pid < 0) {
		printf("fork failed!\n");
		return -1;
	}

	wait(&stat);

	printf("test area = %016lx\n", *(unsigned long *)shstk_buf);

	if (*(unsigned long *)shstk_buf == TEST_PATTERN)
		printf("FAIL\n");
	else
		printf("OK\n");

	return 0;
}
