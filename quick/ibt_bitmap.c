#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <asm/prctl.h>
#include <sys/prctl.h>
#include <x86intrin.h>

int arch_prctl(int code, unsigned long *addr);

#define ARCH_CET_LEGACY_BITMAP 0x3006

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

extern void *__libc_stack_end;

int main(int argc, char *argv[])
{
	int r;
	unsigned long long args[2];
	unsigned char *bitmap;
	unsigned long byte;
	unsigned long bit;
	size_t bitmap_size;

	bitmap_size = ((uintptr_t) __libc_stack_end
		       / sysconf(_SC_PAGESIZE) / 8);
	bitmap = mmap (NULL, bitmap_size, PROT_READ | PROT_WRITE,
		       MAP_ANON | MAP_PRIVATE | MAP_NORESERVE, -1, 0);
	if (bitmap == MAP_FAILED) {
		printf("mmap legacy bitmap failed.\n");
		return -1;
	}

	args[0] = (uintptr_t) bitmap;
	args[1] = bitmap_size;
	r = arch_prctl(ARCH_CET_LEGACY_BITMAP, args);
	if (r) {
		printf("ARCH_CET_LEGACY_BITMAP failed.\n");
		return -1;
	}

	set_bitmap(bitmap, ibt_violation);
	ibt_violation();
	printf("OK\n");
	return 0;
}
