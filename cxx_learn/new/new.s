	.file	"new.cc"
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
	.section	.text._ZnwjPv,"axG",@progbits,operator new(unsigned int, void*),comdat
	.weak	operator new(unsigned int, void*)
	.type	operator new(unsigned int, void*), @function
operator new(unsigned int, void*):
.LFB42:
	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR [ebp+12]
	pop	ebp
	ret
.LFE42:
	.size	operator new(unsigned int, void*), .-operator new(unsigned int, void*)
	.section	.rodata
	.type	std::piecewise_construct, @object
	.size	std::piecewise_construct, 1
std::piecewise_construct:
	.zero	1
	.local	std::__ioinit
	.comm	std::__ioinit,1,1
	.text
	.globl	main
	.type	main, @function
main:
.LFB1389:
	lea	ecx, [esp+4]
	and	esp, -16
	push	DWORD PTR [ecx-4]
	push	ebp
	mov	ebp, esp
	push	edi
	push	esi
	push	ebx
	push	ecx
	sub	esp, 1048
	sub	esp, 12
	push	5204
	call	operator new[](unsigned int)
	add	esp, 16
	mov	ebx, eax
	mov	DWORD PTR [ebx], 13
	lea	eax, [ebx+4]
	mov	edi, 12
	mov	esi, eax
.L7:
	cmp	edi, -1
	je	.L6
	sub	esp, 12
	push	esi
	call	A::A()
	add	esp, 16
	add	esi, 400
	sub	edi, 1
	jmp	.L7
.L6:
	lea	eax, [ebx+4]
	mov	DWORD PTR [ebp-28], eax
#APP
# 24 "new.cc" 1
	nop
# 0 "" 2
#NO_APP
	sub	esp, 12
	push	400
	call	operator new(unsigned int)
	add	esp, 16
	mov	ebx, eax
	sub	esp, 12
	push	ebx
	call	A::A()
	add	esp, 16
	mov	DWORD PTR [ebp-32], ebx
#APP
# 27 "new.cc" 1
	nop
# 0 "" 2
#NO_APP
	sub	esp, 8
	push	OFFSET FLAT:std::nothrow
	push	400
	call	operator new(unsigned int, std::nothrow_t const&)
	add	esp, 16
	mov	ebx, eax
	test	ebx, ebx
	je	.L8
	sub	esp, 12
	push	ebx
	call	A::A()
	add	esp, 16
	mov	eax, ebx
	jmp	.L9
.L8:
	mov	eax, ebx
.L9:
	mov	DWORD PTR [ebp-36], eax
#APP
# 30 "new.cc" 1
	nop
# 0 "" 2
#NO_APP
	lea	eax, [ebp-1064]
	sub	esp, 8
	push	eax
	push	400
	call	operator new(unsigned int, void*)
	add	esp, 16
	mov	ebx, eax
	test	ebx, ebx
	je	.L10
	sub	esp, 12
	push	ebx
	call	A::A()
	add	esp, 16
	mov	eax, ebx
	jmp	.L11
.L10:
	mov	eax, ebx
.L11:
	mov	DWORD PTR [ebp-40], eax
#APP
# 33 "new.cc" 1
	nop
# 0 "" 2
#NO_APP
	mov	ebx, DWORD PTR [ebp-32]
	test	ebx, ebx
	je	.L12
	sub	esp, 12
	push	ebx
	call	A::~A()
	add	esp, 16
	sub	esp, 12
	push	ebx
	call	operator delete(void*)
	add	esp, 16
.L12:
#APP
# 36 "new.cc" 1
	nop
# 0 "" 2
#NO_APP
	mov	ebx, DWORD PTR [ebp-36]
	test	ebx, ebx
	je	.L13
	sub	esp, 12
	push	ebx
	call	A::~A()
	add	esp, 16
	sub	esp, 12
	push	ebx
	call	operator delete(void*)
	add	esp, 16
.L13:
#APP
# 39 "new.cc" 1
	nop
# 0 "" 2
#NO_APP
	sub	esp, 12
	push	DWORD PTR [ebp-40]
	call	A::~A()
	add	esp, 16
	mov	eax, 0
	lea	esp, [ebp-16]
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	pop	ebp
	lea	esp, [ecx-4]
	ret
.LFE1389:
	.size	main, .-main
	.type	__static_initialization_and_destruction_0(int, int), @function
__static_initialization_and_destruction_0(int, int):
.LFB1570:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	cmp	DWORD PTR [ebp+8], 1
	jne	.L17
	cmp	DWORD PTR [ebp+12], 65535
	jne	.L17
	sub	esp, 12
	push	OFFSET FLAT:std::__ioinit
	call	std::ios_base::Init::Init()
	add	esp, 16
	sub	esp, 4
	push	OFFSET FLAT:__dso_handle
	push	OFFSET FLAT:std::__ioinit
	push	OFFSET FLAT:std::ios_base::Init::~Init()
	call	__cxa_atexit
	add	esp, 16
.L17:
	nop
	leave
	ret
.LFE1570:
	.size	__static_initialization_and_destruction_0(int, int), .-__static_initialization_and_destruction_0(int, int)
	.type	_GLOBAL__sub_I_main, @function
_GLOBAL__sub_I_main:
.LFB1571:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	sub	esp, 8
	push	65535
	push	1
	call	__static_initialization_and_destruction_0(int, int)
	add	esp, 16
	leave
	ret
.LFE1571:
	.size	_GLOBAL__sub_I_main, .-_GLOBAL__sub_I_main
	.section	.init_array,"aw"
	.align 4
	.long	_GLOBAL__sub_I_main
	.hidden	__dso_handle
	.ident	"GCC: (GNU) 5.3.1 20160406 (Red Hat 5.3.1-6)"
	.section	.note.GNU-stack,"",@progbits
