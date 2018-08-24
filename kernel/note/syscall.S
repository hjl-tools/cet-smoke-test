	.text
	.type syscall,  @function
	.globl syscall
	.p2align 4
syscall:
	.cfi_startproc
#ifdef __x86_64__
	movq %rdi, %rax		/* Syscall number -> rax.  */
	movq %rsi, %rdi		/* shift arg1 - arg5.  */
	movq %rdx, %rsi
	movq %rcx, %rdx
	movq %r8, %r10
	movq %r9, %r8
	movq 8(%rsp),%r9	/* arg6 is on the stack.  */
	syscall			/* Do the system call.  */
#else
	push %ebp
	push %edi
	push %esi
	push %ebx
	mov 0x2c(%esp),%ebp
	mov 0x28(%esp),%edi
	mov 0x24(%esp),%esi
	mov 0x20(%esp),%edx
	mov 0x1c(%esp),%ecx
	mov 0x18(%esp),%ebx
	mov 0x14(%esp),%eax
	int $0x80
	pop %ebx
	pop %esi
	pop %edi
	pop %ebp
#endif
	ret			/* Return to caller.  */
	.cfi_endproc
	.size syscall, .-syscall

