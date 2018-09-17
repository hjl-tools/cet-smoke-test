/* SPDX-License-Identifier: GPL-2.0 */
/* Quick tests to verify Shadow Stack and IBT are working */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <ucontext.h>

ucontext_t ucp;
int result[4] = {-1, -1, -1, -1};
int test_id;

void stack_hacked(unsigned long x)
{
	result[test_id] = -1;
	test_id++;
	setcontext(&ucp);
}

void ibt_violation(void)
{
	asm volatile("lea 1f, %rax");
	asm volatile("jmp *%rax");
	asm volatile("1:");
	result[test_id] = -1;
	test_id++;
	setcontext(&ucp);
}

void shstk_violation(void)
{
#ifdef __i386__
	unsigned long x[1];

	x[2] = (unsigned long)stack_hacked;
#else
	unsigned long long x[1];

	x[2] = (unsigned long long)(unsigned long)stack_hacked;
#endif
}

void segv_handler(int signum, siginfo_t *si, void *uc)
{
	result[test_id] = 0;
	test_id++;
	setcontext(&ucp);
}

void user1_handler(int signum, siginfo_t *si, void *uc)
{
	shstk_violation();
}

void user2_handler(int signum, siginfo_t *si, void *uc)
{
	ibt_violation();
}

int main(int argc, char *argv[])
{
	struct sigaction sa;
	int r;

	r = sigemptyset(&sa.sa_mask);
	if (r)
		return -1;

	sa.sa_flags = SA_SIGINFO;

	/*
	 * Control protection fault handler
	 */
	sa.sa_sigaction = segv_handler;
	r = sigaction(SIGSEGV, &sa, NULL);
	if (r)
		return -1;

	/*
	 * Handler to test SHSTK
	 */
	sa.sa_sigaction = user1_handler;
	r = sigaction(SIGUSR1, &sa, NULL);
	if (r)
		return -1;

	/*
	 * Handler to test IBT
	 */
	sa.sa_sigaction = user2_handler;
	r = sigaction(SIGUSR2, &sa, NULL);
	if (r)
		return -1;

	test_id = 0;
	r = getcontext(&ucp);
	if (r)
		return -1;

	if (test_id == 0)
		shstk_violation();
	else if (test_id == 1)
		ibt_violation();
	else if (test_id == 2)
		raise(SIGUSR1);
	else if (test_id == 3)
		raise(SIGUSR2);

	r = 0;
	printf("SHSTK: %s\n", result[0] ? "FAIL":"OK");
	r += result[0];
	printf("IBT:   %s\n", result[1] ? "FAIL":"OK");
	r += result[1];
	printf("SHSTK in signal: %s\n", result[2] ? "FAIL":"OK");
	r += result[2];
	printf("IBT in signal:   %s\n", result[3] ? "FAIL":"OK");
	r += result[3];
	return r;
}
