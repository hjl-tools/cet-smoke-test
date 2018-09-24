/* SPDX-License-Identifier: GPL-2.0 */
/* Quick tests to verify PTRACE_POKEDATA can write to a shadow stack */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <x86intrin.h>

#if defined(__i386__) || (__SIZEOF_LONG__ == 4)
#define TEST_PATTERN 0xa5a5a5a5UL
#else
#define TEST_PATTERN 0xa5a5a5a5a5a5a5a5UL
#endif

void *get_last_shstk_page(void)
{
	void *shstk_page;

	shstk_page = (void *)_get_ssp();
	shstk_page = (void *)((unsigned long)shstk_page &
			(unsigned long) -0x1000);
	return shstk_page;
}

int main(int argc, char *argv[])
{
	int r, status;
	pid_t child;
	void *shstk_page;
	unsigned long data;

	child = fork();
	if (child == 0) {
		sleep(1);
		shstk_page = get_last_shstk_page();
//		printf("child: shstk page = %p\n", shstk_page);
//		printf("child: *shstk_page=%016lx\n",
//		       *(unsigned long *)shstk_page);
		if (*(unsigned long *)shstk_page == TEST_PATTERN)
			printf("OK\n");
		else
			printf("FAIL\n");
		exit(0);
	}

	if (child < 0) {
		printf("fork failed!\n");
		return -1;
	}

	shstk_page = get_last_shstk_page();
//	printf("self: shstk page = %p\n", shstk_page);
//	printf("self: *shstk_page=%016lx\n",
//	       *(unsigned long *)shstk_page);

	r = ptrace(PTRACE_ATTACH, child, NULL, NULL);
	if (r) {
		printf("PTRACE_ATTACH failed!\n");
		return -1;
	}

	r = waitpid(child, &status, 0);
	if (r != child) {
		printf("waitpid failed!\n");
		return -1;
	}

	data = TEST_PATTERN;
	r = ptrace(PTRACE_POKEDATA, child, shstk_page, data);
	if (r) {
		printf("PTRACE_POKEDATA failed!\n");
		return -1;
	}

	ptrace(PTRACE_CONT, child, NULL, NULL);
	return 0;
}
