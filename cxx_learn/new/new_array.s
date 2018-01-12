	.file	"new_array.cc"
	.intel_syntax noprefix
	.section	.text._ZN1AC2Ev,"axG",@progbits,A::A(),comdat
	.align 2
	.weak	A::A()
	.type	A::A(), @function
A::A():
.LFB1:
	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR [ebp+8]
	mov	DWORD PTR [eax], 123
	nop
	pop	ebp
	ret
.LFE1:
	.size	A::A(), .-A::A()
	.weak	A::A()
	.set	A::A(),A::A()
	.section	.text._ZN1AD2Ev,"axG",@progbits,A::~A(),comdat
	.align 2
	.weak	A::~A()
	.type	A::~A(), @function
A::~A():
.LFB4:
	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR [ebp+8]
	mov	DWORD PTR [eax], 321
	nop
	pop	ebp
	ret
.LFE4:
	.size	A::~A(), .-A::~A()
	.weak	A::~A()
	.set	A::~A(),A::~A()
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	lea	ecx, [esp+4]
	and	esp, -16
	push	DWORD PTR [ecx-4]
	push	ebp
	mov	ebp, esp
	push	edi
	push	esi
	push	ebx
	push	ecx
	sub	esp, 24
#APP
# 19 "new_array.cc" 1
	nop
# 0 "" 2
#NO_APP
	sub	esp, 12
	push	400
	call	operator new[](unsigned int)
	add	esp, 16
	mov	DWORD PTR [ebp-28], eax
#APP
# 22 "new_array.cc" 1
	nop
# 0 "" 2
#NO_APP
	cmp	DWORD PTR [ebp-28], 0
	je	.L4
	sub	esp, 12
	push	DWORD PTR [ebp-28]
	call	operator delete[](void*)
	add	esp, 16
.L4:
#APP
# 25 "new_array.cc" 1
	nop
# 0 "" 2
#NO_APP
	sub	esp, 12
	push	49204
	call	operator new[](unsigned int)
	add	esp, 16
	mov	ebx, eax
	mov	DWORD PTR [ebx], 123
	lea	eax, [ebx+4]
	mov	edi, 122
	mov	esi, eax
.L6:
	cmp	edi, -1
	je	.L5
	sub	esp, 12
	push	esi
	call	A::A()
	add	esp, 16
	add	esi, 400
	sub	edi, 1
	jmp	.L6
.L5:
	lea	eax, [ebx+4]
	mov	DWORD PTR [ebp-32], eax
#APP
# 28 "new_array.cc" 1
	nop
# 0 "" 2
#NO_APP
	cmp	DWORD PTR [ebp-32], 0
	je	.L7
	mov	eax, DWORD PTR [ebp-32]
	sub	eax, 4
	mov	eax, DWORD PTR [eax]
	imul	edx, eax, 400
	mov	eax, DWORD PTR [ebp-32]
	lea	ebx, [edx+eax]
.L9:
	cmp	ebx, DWORD PTR [ebp-32]
	je	.L8
	sub	ebx, 400
	sub	esp, 12
	push	ebx
	call	A::~A()
	add	esp, 16
	jmp	.L9
.L8:
	mov	eax, DWORD PTR [ebp-32]
	sub	eax, 4
	sub	esp, 12
	push	eax
	call	operator delete[](void*)
	add	esp, 16
.L7:
	mov	eax, 0
	lea	esp, [ebp-16]
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	pop	ebp
	lea	esp, [ecx-4]
	ret
.LFE6:
	.size	main, .-main
	.ident	"GCC: (GNU) 5.3.1 20160406 (Red Hat 5.3.1-6)"
	.section	.note.GNU-stack,"",@progbits
