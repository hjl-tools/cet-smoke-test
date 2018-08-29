	.text
	.globl _start
	.type _start,@function
_start:
	xorl %ebp, %ebp
#ifdef __LP64__
	popq %rdi
	movq %rsp, %rsi
	andq  $~15, %rsp
#elif defined __x86_64__
	mov (%rsp),%edi
	addl $4,%esp
	movl %esp, %esi
	andl  $~15, %esp
#else
	popl %esi
	movl %esp, %ecx
	andl  $~15, %esp

	subl $8,%esp
	pushl %ecx
	pushl %esi
#endif

	call main

	hlt
