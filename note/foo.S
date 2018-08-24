	.text
	.globl	foo
	.type	foo, @function
foo:
	.cfi_startproc
#ifdef __x86_64__
	movq	(%rsp), %rax
	addq	$8, %rsp
	jmp	*%rax
#else
	movl	(%esp), %eax
	addl	$4, %esp
	jmp	*%eax
#endif
	.cfi_endproc
	.size	foo, .-foo
	.section	.note.GNU-stack,"",@progbits
