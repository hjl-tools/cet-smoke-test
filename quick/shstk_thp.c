/* SPDX-License-Identifier: GPL-2.0 */
/* Verify a shadow stack transparent huge page after fork() */

#include <stdio.h>
#include <stdlib.h>
#include <asm/prctl.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>

#define ARCH_CET_STATUS 0x3001

int arch_prctl(int code, unsigned long *addr);

/*
 * Fill up the shadow stack up to the THP.
 * Two functions are idential, but the addresses differ.
 * This will detect any shared shadow stack pages.
 */
void call_to_self1(int total)
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

void call_to_self2(int total)
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
	unsigned long long stat[3];
	pid_t child;
	int r;

	r = arch_prctl(ARCH_CET_STATUS, stat);
	if (r) {
		printf("ARCH_CET_STATUS failed!\n");
		return -1;
	}

	printf("self pid = %d, shstk_base = %016lx, size = %016lx\n",
	       getpid(), (long) stat[1], (long) stat[2]);

	if (stat[2] < 0x300000) {
		printf("Shadow stack must be larger then 3 MB\n");
		return 0;
	}

	/*
	 * Try to trigger the shadow stack as THPs, but don't want
	 * to get into a #CP.  Leave out the current page.
	 */
	r = madvise((void *)(stat[1]), (size_t)stat[2], MADV_HUGEPAGE);
	if (r) {
		printf("MADV_HUGEPAGE failed!\n");
		return -1;
	}

	madvise((void *)(stat[1]), (size_t)stat[2] - 0x1000, MADV_DONTNEED);
	if (r) {
		printf("MADV_DONTNEED failed!\n");
		return -1;
	}

	madvise((void *)(stat[1]), (size_t)stat[2] - 0x1000, MADV_WILLNEED);
	if (r) {
		printf("MADV_WILLNEED failed!\n");
		return -1;
	}

	child = fork();

	if (child == 0) {
		printf("child pid = %d\n", getpid());
		call_to_self2(0x200000/8);
		exit(0);
	}

	if (child < 0) {
		printf("fork failed!\n");
		return -1;
	}

	call_to_self1(0x200000/8);
	return 0;
}
