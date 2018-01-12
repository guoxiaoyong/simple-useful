	.file	"b.cc"
	.text
	.globl	filter_only()
	.type	filter_only(), @function
filter_only():
.LFB0:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA0
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
.LEHB0:
	call	ext_func()
.LEHE0:
	jmp	.L5
.L4:
	cmpq	$-1, %rdx
	je	.L3
	movq	%rax, %rdi
.LEHB1:
	call	_Unwind_Resume
.L3:
	movq	%rax, %rdi
	call	__cxa_call_unexpected
.LEHE1:
.L5:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
	.align 4
.LLSDA0:
	.byte	0xff
	.byte	0x3
	.uleb128 .LLSDATT0-.LLSDATTD0
.LLSDATTD0:
	.byte	0x1
	.uleb128 .LLSDACSE0-.LLSDACSB0
.LLSDACSB0:
	.uleb128 .LEHB0-.LFB0
	.uleb128 .LEHE0-.LEHB0
	.uleb128 .L4-.LFB0
	.uleb128 0x1
	.uleb128 .LEHB1-.LFB0
	.uleb128 .LEHE1-.LEHB1
	.uleb128 0
	.uleb128 0
.LLSDACSE0:
	.byte	0x7f
	.byte	0
	.align 4
	.long	typeinfo for ExcA
	.long	typeinfo for ExcB
	.long	typeinfo for ExcC
	.long	typeinfo for ExcD
	.long	typeinfo for ExcE
.LLSDATT0:
	.byte	0x1
	.byte	0x2
	.byte	0x3
	.byte	0x4
	.byte	0x5
	.byte	0
	.text
	.size	filter_only(), .-filter_only()
	.weak	typeinfo for ExcE
	.section	.rodata._ZTI4ExcE,"aG",@progbits,typeinfo for ExcE,comdat
	.align 8
	.type	typeinfo for ExcE, @object
	.size	typeinfo for ExcE, 16
typeinfo for ExcE:
	.quad	vtable for __cxxabiv1::__class_type_info+16
	.quad	typeinfo name for ExcE
	.weak	typeinfo name for ExcE
	.section	.rodata._ZTS4ExcE,"aG",@progbits,typeinfo name for ExcE,comdat
	.type	typeinfo name for ExcE, @object
	.size	typeinfo name for ExcE, 6
typeinfo name for ExcE:
	.string	"4ExcE"
	.weak	typeinfo for ExcD
	.section	.rodata._ZTI4ExcD,"aG",@progbits,typeinfo for ExcD,comdat
	.align 8
	.type	typeinfo for ExcD, @object
	.size	typeinfo for ExcD, 16
typeinfo for ExcD:
	.quad	vtable for __cxxabiv1::__class_type_info+16
	.quad	typeinfo name for ExcD
	.weak	typeinfo name for ExcD
	.section	.rodata._ZTS4ExcD,"aG",@progbits,typeinfo name for ExcD,comdat
	.type	typeinfo name for ExcD, @object
	.size	typeinfo name for ExcD, 6
typeinfo name for ExcD:
	.string	"4ExcD"
	.weak	typeinfo for ExcC
	.section	.rodata._ZTI4ExcC,"aG",@progbits,typeinfo for ExcC,comdat
	.align 8
	.type	typeinfo for ExcC, @object
	.size	typeinfo for ExcC, 16
typeinfo for ExcC:
	.quad	vtable for __cxxabiv1::__class_type_info+16
	.quad	typeinfo name for ExcC
	.weak	typeinfo name for ExcC
	.section	.rodata._ZTS4ExcC,"aG",@progbits,typeinfo name for ExcC,comdat
	.type	typeinfo name for ExcC, @object
	.size	typeinfo name for ExcC, 6
typeinfo name for ExcC:
	.string	"4ExcC"
	.weak	typeinfo for ExcB
	.section	.rodata._ZTI4ExcB,"aG",@progbits,typeinfo for ExcB,comdat
	.align 8
	.type	typeinfo for ExcB, @object
	.size	typeinfo for ExcB, 16
typeinfo for ExcB:
	.quad	vtable for __cxxabiv1::__class_type_info+16
	.quad	typeinfo name for ExcB
	.weak	typeinfo name for ExcB
	.section	.rodata._ZTS4ExcB,"aG",@progbits,typeinfo name for ExcB,comdat
	.type	typeinfo name for ExcB, @object
	.size	typeinfo name for ExcB, 6
typeinfo name for ExcB:
	.string	"4ExcB"
	.weak	typeinfo for ExcA
	.section	.rodata._ZTI4ExcA,"aG",@progbits,typeinfo for ExcA,comdat
	.align 8
	.type	typeinfo for ExcA, @object
	.size	typeinfo for ExcA, 16
typeinfo for ExcA:
	.quad	vtable for __cxxabiv1::__class_type_info+16
	.quad	typeinfo name for ExcA
	.weak	typeinfo name for ExcA
	.section	.rodata._ZTS4ExcA,"aG",@progbits,typeinfo name for ExcA,comdat
	.type	typeinfo name for ExcA, @object
	.size	typeinfo name for ExcA, 6
typeinfo name for ExcA:
	.string	"4ExcA"
	.ident	"GCC: (GNU) 5.3.1 20160406 (Red Hat 5.3.1-6)"
	.section	.note.GNU-stack,"",@progbits
