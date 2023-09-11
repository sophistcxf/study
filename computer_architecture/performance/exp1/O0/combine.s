	.file	"combine.cpp"
	.text
	.globl	_Z10vec_lengthP7vec_rec
	.type	_Z10vec_lengthP7vec_rec, @function
_Z10vec_lengthP7vec_rec:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	_Z10vec_lengthP7vec_rec, .-_Z10vec_lengthP7vec_rec
	.globl	_Z13get_vec_startP7vec_rec
	.type	_Z13get_vec_startP7vec_rec, @function
_Z13get_vec_startP7vec_rec:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	_Z13get_vec_startP7vec_rec, .-_Z13get_vec_startP7vec_rec
	.globl	_Z8combine2P7vec_recPl
	.type	_Z8combine2P7vec_recPl, @function
_Z8combine2P7vec_recPl:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	_Z8combine2P7vec_recPl, .-_Z8combine2P7vec_recPl
	.globl	_Z8combine3P7vec_recPl
	.type	_Z8combine3P7vec_recPl, @function
_Z8combine3P7vec_recPl:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	_Z10vec_lengthP7vec_rec
	movq	%rax, -16(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	_Z13get_vec_startP7vec_rec
	movq	%rax, -24(%rbp)
	movq	-48(%rbp), %rax
	movq	$1, (%rax)
	movq	$0, -8(%rbp)
.L8:
	movq	-8(%rbp), %rax
	cmpq	-16(%rbp), %rax
	jge	.L9
	movq	-48(%rbp), %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	leaq	0(,%rax,8), %rcx
	movq	-24(%rbp), %rax
	addq	%rcx, %rax
	movq	(%rax), %rax
	imulq	%rax, %rdx
	movq	-48(%rbp), %rax
	movq	%rdx, (%rax)
	addq	$1, -8(%rbp)
	jmp	.L8
.L9:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	_Z8combine3P7vec_recPl, .-_Z8combine3P7vec_recPl
	.globl	_Z8combine4P7vec_recPl
	.type	_Z8combine4P7vec_recPl, @function
_Z8combine4P7vec_recPl:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	_Z10vec_lengthP7vec_rec
	movq	%rax, -24(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	_Z13get_vec_startP7vec_rec
	movq	%rax, -32(%rbp)
	movq	$1, -16(%rbp)
	movq	$0, -8(%rbp)
.L12:
	movq	-8(%rbp), %rax
	cmpq	-24(%rbp), %rax
	jge	.L11
	movq	-8(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	-16(%rbp), %rdx
	imulq	%rdx, %rax
	movq	%rax, -16(%rbp)
	addq	$1, -8(%rbp)
	jmp	.L12
.L11:
	movq	-48(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, (%rax)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	_Z8combine4P7vec_recPl, .-_Z8combine4P7vec_recPl
	.globl	main
	.type	main, @function
main:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.5.0 20210514 (Red Hat 8.5.0-4)"
	.section	.note.GNU-stack,"",@progbits
