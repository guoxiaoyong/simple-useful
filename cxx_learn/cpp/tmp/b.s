	.file	"b.cc"
	.text
	.globl	_Z11filter_onlyv
	.type	_Z11filter_onlyv, @function
_Z11filter_onlyv:
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
	call	_Z8ext_funcv
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
	.long	_ZTI4ExcA
	.long	_ZTI4ExcB
	.long	_ZTI4ExcC
	.long	_ZTI4ExcD
	.long	_ZTI4ExcE
.LLSDATT0:
	.byte	0x1
	.byte	0x2
	.byte	0x3
	.byte	0x4
	.byte	0x5
	.byte	0
	.text
	.size	_Z11filter_onlyv, .-_Z11filter_onlyv
	.weak	_ZTI4ExcE
	.section	.rodata._ZTI4ExcE,"aG",@progbits,_ZTI4ExcE,comdat
	.align 8
	.type	_ZTI4ExcE, @object
	.size	_ZTI4ExcE, 16
_ZTI4ExcE:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTS4ExcE
	.weak	_ZTS4ExcE
	.section	.rodata._ZTS4ExcE,"aG",@progbits,_ZTS4ExcE,comdat
	.type	_ZTS4ExcE, @object
	.size	_ZTS4ExcE, 6
_ZTS4ExcE:
	.string	"4ExcE"
	.weak	_ZTI4ExcD
	.section	.rodata._ZTI4ExcD,"aG",@progbits,_ZTI4ExcD,comdat
	.align 8
	.type	_ZTI4ExcD, @object
	.size	_ZTI4ExcD, 16
_ZTI4ExcD:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTS4ExcD
	.weak	_ZTS4ExcD
	.section	.rodata._ZTS4ExcD,"aG",@progbits,_ZTS4ExcD,comdat
	.type	_ZTS4ExcD, @object
	.size	_ZTS4ExcD, 6
_ZTS4ExcD:
	.string	"4ExcD"
	.weak	_ZTI4ExcC
	.section	.rodata._ZTI4ExcC,"aG",@progbits,_ZTI4ExcC,comdat
	.align 8
	.type	_ZTI4ExcC, @object
	.size	_ZTI4ExcC, 16
_ZTI4ExcC:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTS4ExcC
	.weak	_ZTS4ExcC
	.section	.rodata._ZTS4ExcC,"aG",@progbits,_ZTS4ExcC,comdat
	.type	_ZTS4ExcC, @object
	.size	_ZTS4ExcC, 6
_ZTS4ExcC:
	.string	"4ExcC"
	.weak	_ZTI4ExcB
	.section	.rodata._ZTI4ExcB,"aG",@progbits,_ZTI4ExcB,comdat
	.align 8
	.type	_ZTI4ExcB, @object
	.size	_ZTI4ExcB, 16
_ZTI4ExcB:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTS4ExcB
	.weak	_ZTS4ExcB
	.section	.rodata._ZTS4ExcB,"aG",@progbits,_ZTS4ExcB,comdat
	.type	_ZTS4ExcB, @object
	.size	_ZTS4ExcB, 6
_ZTS4ExcB:
	.string	"4ExcB"
	.weak	_ZTI4ExcA
	.section	.rodata._ZTI4ExcA,"aG",@progbits,_ZTI4ExcA,comdat
	.align 8
	.type	_ZTI4ExcA, @object
	.size	_ZTI4ExcA, 16
_ZTI4ExcA:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTS4ExcA
	.weak	_ZTS4ExcA
	.section	.rodata._ZTS4ExcA,"aG",@progbits,_ZTS4ExcA,comdat
	.type	_ZTS4ExcA, @object
	.size	_ZTS4ExcA, 6
_ZTS4ExcA:
	.string	"4ExcA"
	.ident	"GCC: (GNU) 5.3.1 20160406 (Red Hat 5.3.1-6)"
	.section	.note.GNU-stack,"",@progbits
