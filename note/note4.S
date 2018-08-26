	.section ".note", "a"
#ifdef __LP64__
	.p2align 3
#else
	.p2align 2
#endif
	.long 1f - 0f		/* name length.  */
	.long 3f - 1f		/* data length.  */
	/* NT_GNU_PROPERTY_TYPE_0 */
	.long 5			/* note type.  */
0:
	.asciz "GNU"		/* vendor name.  */
1:
#ifdef __LP64__
	.p2align 3
#else
	.p2align 2
#endif
	/* GNU_PROPERTY_X86_FEATURE_1_AND */
	.long 0xc0000002	/* pr_type.  */
	.long 5f - 4f		/* pr_datasz.  */
4:
	.long 0x80000003
5:
#ifdef __LP64__
	.p2align 3
#else
	.p2align 2
#endif
3:
