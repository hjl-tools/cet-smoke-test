#include <stdio.h>
#include <stdlib.h>
#include <asm/prctl.h>
#include <sys/prctl.h>

int arch_prctl(int code, unsigned long *addr);

#define ARCH_CET_ALLOC_SHSTK	0x3004

int main(int argc, char *argv[])
{
	int err;
	unsigned long arg;
	unsigned long ssp_x;
	unsigned long ssp_y;

	asm volatile("RDSSPQ %0\n": "=r" (ssp_x));
	printf("ssp_x = %016lx\n", ssp_x);

	arg = 0x1000;
	err = arch_prctl(ARCH_CET_ALLOC_SHSTK, &arg);

	if (err) {
		printf("ARCH_CET_ALLOC_SHSTK failed!\n");
		return -1;
	}

	printf("allocated ssp = %p, size = %x\n",
	       arg, 0x1000);
	ssp_y = arg + 0x1000 - 8;
	printf("ssp_y = %016lx; *ssp_y=%016lx\n",
	       ssp_y, *(unsigned long *)ssp_y);

	asm volatile("RSTORSSP (%0)\n":: "r" (ssp_y));
	asm volatile("SAVEPREVSSP");

	asm volatile("RDSSPQ %0\n": "=r" (ssp_y));
	printf("confirm ssp_y = %016lx\n", ssp_y);

	ssp_x -= 8;
	asm volatile("RSTORSSP (%0)\n":: "r" (ssp_x));
	asm volatile("SAVEPREVSSP");

	asm volatile("RDSSPQ %0\n": "=r" (ssp_x));
	printf("confirm ssp_x = %016lx\n", ssp_x);

	printf("done!\n");
	return 0;
}

