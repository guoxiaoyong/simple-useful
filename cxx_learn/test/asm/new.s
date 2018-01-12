	.file	"g.cc"
	.intel_syntax noprefix
	.text
	.globl	fun(int&&)
	.type	fun(int&&), @function
fun(int&&):
	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR [ebp+8]
	mov	DWORD PTR [eax], 10
	nop
	pop	ebp
	ret
	.size	fun(int&&), .-fun(int&&)
	.ident	"GCC: (GNU) 5.3.1 20160406 (Red Hat 5.3.1-6)"
	.section	.note.GNU-stack,"",@progbits
