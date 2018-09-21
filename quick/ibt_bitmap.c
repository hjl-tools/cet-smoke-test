#include <stdio.h>
#include <stdlib.h>
#include <asm/prctl.h>
#include <sys/prctl.h>
#include <x86intrin.h>

int arch_prctl(int code, unsigned long *addr);

#define ARCH_CET_LEGACY_BITMAP 0x3005

void ibt_violation(void)
{
#ifdef __i386__
	asm volatile("lea 1f, %eax");
	asm volatile("jmp *%eax");
#else
	asm volatile("lea 1f, %rax");
	asm volatile("jmp *%rax");
#endif
	asm volatile("1:");
}

void set_bitmap(unsigned char *bitmap, void *addr)
{
	unsigned long page = (unsigned long)addr / 0x1000;
	unsigned long byte = page / 8;
	unsigned long bit = page % 8;
	bitmap[byte] |= (0x01 << bit);
}

int main(int argc, char *argv[])
{
	int r;
	unsigned long args[2];
	unsigned char *bitmap;
	unsigned long byte;
	unsigned long bit;

	r = arch_prctl(ARCH_CET_LEGACY_BITMAP, args);
	if (r) {
		printf("ARCH_CET_LEGACY_BITMAP failed.\n");
		return -1;
	}

	if (args[0] == 0) {
		printf("Bitmap.is NULL.\n");
		return -1;
	}

	bitmap = (unsigned char *)args[0];
	set_bitmap(bitmap, ibt_violation);
	ibt_violation();
	printf("OK\n");
	return 0;
}
