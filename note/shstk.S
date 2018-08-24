	.text
	.p2align 4,,15
	.type	bar, @function
bar:
.LFB0:
	.cfi_startproc
	call	foo
#ifdef __x86_64__
	endbr64
#else
	endbr32
#endif
	ret
	.cfi_endproc
.LFE0:
	.size	bar, .-bar
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
#ifdef __x86_64__
	endbr64
#else
	endbr32
#endif
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	bar
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
