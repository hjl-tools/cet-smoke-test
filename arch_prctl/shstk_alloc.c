#define _GNU_SOURCE
#include <unistd.h>
#include <syscall.h>
#include <asm/prctl.h>

#ifndef ARCH_CET_STATUS
# define ARCH_CET_ALLOC_SHSTK	0x3004
#endif

static int
length (const char *s)
{
  int l;

  for (l = 0; *s++ != 0; l++);
  return l;
}

static void
msg (const char *str)
{
  int fd = STDOUT_FILENO;
  syscall (SYS_write, fd, str, length (str));
}

int main(int argc, char *argv[])
{
	int err;
	unsigned long arg;
	unsigned long ssp_x;
	unsigned long ssp_y;

	asm volatile("RDSSPQ %0\n": "=r" (ssp_x));

	arg = 0x1000;
	err = syscall(SYS_arch_prctl, ARCH_CET_ALLOC_SHSTK, &arg);

	if (err) {
		msg("ARCH_CET_ALLOC_SHSTK failed!\n");
		syscall (SYS_exit, -1);
	}

	ssp_y = arg + 0x1000 - 8;

	asm volatile("RSTORSSP (%0)\n":: "r" (ssp_y));
	asm volatile("SAVEPREVSSP");

	asm volatile("RDSSPQ %0\n": "=r" (ssp_y));

	ssp_x -= 8;
	asm volatile("RSTORSSP (%0)\n":: "r" (ssp_x));
	asm volatile("SAVEPREVSSP");

	asm volatile("RDSSPQ %0\n": "=r" (ssp_x));

	msg ("ARCH_CET_ALLOC_SHSTK is OK!\n");
	syscall (SYS_exit, 0);
	return 0;
}

