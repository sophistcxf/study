	.file	"combine.cpp"
	.text
	.globl	_Z10vec_lengthP7vec_rec
	.type	_Z10vec_lengthP7vec_rec, @function
_Z10vec_lengthP7vec_rec:
.LFB0:
	.cfi_startproc
	movq	(%rdi), %rax
	ret
	.cfi_endproc
.LFE0:
	.size	_Z10vec_lengthP7vec_rec, .-_Z10vec_lengthP7vec_rec
	.globl	_Z13get_vec_startP7vec_rec
	.type	_Z13get_vec_startP7vec_rec, @function
_Z13get_vec_startP7vec_rec:
.LFB1:
	.cfi_startproc
	movq	8(%rdi), %rax
	ret
	.cfi_endproc
.LFE1:
	.size	_Z13get_vec_startP7vec_rec, .-_Z13get_vec_startP7vec_rec
	.globl	_Z8combine2P7vec_recPl
	.type	_Z8combine2P7vec_recPl, @function
_Z8combine2P7vec_recPl:
.LFB2:
	.cfi_startproc
	ret
	.cfi_endproc
.LFE2:
	.size	_Z8combine2P7vec_recPl, .-_Z8combine2P7vec_recPl
	.globl	_Z8combine3P7vec_recPl
	.type	_Z8combine3P7vec_recPl, @function
_Z8combine3P7vec_recPl:
.LFB3:
	.cfi_startproc
	movq	(%rdi), %rdx
	movq	8(%rdi), %rcx
	movq	$1, (%rsi)
	testq	%rdx, %rdx
	jle	.L4
	movq	%rcx, %rax
	leaq	(%rcx,%rdx,8), %rcx
.L6:
	movq	(%rsi), %rdx
	imulq	(%rax), %rdx
	movq	%rdx, (%rsi)
	addq	$8, %rax
	cmpq	%rcx, %rax
	jne	.L6
.L4:
	ret
	.cfi_endproc
.LFE3:
	.size	_Z8combine3P7vec_recPl, .-_Z8combine3P7vec_recPl
	.globl	_Z8combine4P7vec_recPl
	.type	_Z8combine4P7vec_recPl, @function
_Z8combine4P7vec_recPl:
.LFB4:
	.cfi_startproc
	movq	(%rdi), %rdx
	movq	8(%rdi), %rcx
	testq	%rdx, %rdx
	jle	.L11
	movq	%rcx, %rax
	leaq	(%rcx,%rdx,8), %rcx
	movl	$1, %edx
.L10:
	imulq	(%rax), %rdx
	addq	$8, %rax
	cmpq	%rcx, %rax
	jne	.L10
.L9:
	movq	%rdx, (%rsi)
	ret
.L11:
	movl	$1, %edx
	jmp	.L9
	.cfi_endproc
.LFE4:
	.size	_Z8combine4P7vec_recPl, .-_Z8combine4P7vec_recPl
	.globl	main
	.type	main, @function
main:
.LFB5:
	.cfi_startproc
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE5:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.5.0 20210514 (Red Hat 8.5.0-4)"
	.section	.note.GNU-stack,"",@progbits
