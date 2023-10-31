	.file	"combine.cpp"
	.text
	.p2align 4,,15
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
	.p2align 4,,15
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
	.p2align 4,,15
	.globl	_Z8combine2P7vec_recPl
	.type	_Z8combine2P7vec_recPl, @function
_Z8combine2P7vec_recPl:
.LFB2:
	.cfi_startproc
	ret
	.cfi_endproc
.LFE2:
	.size	_Z8combine2P7vec_recPl, .-_Z8combine2P7vec_recPl
	.p2align 4,,15
	.globl	_Z8combine3P7vec_recPl
	.type	_Z8combine3P7vec_recPl, @function
_Z8combine3P7vec_recPl:
.LFB3:
	.cfi_startproc
	movq	(%rdi), %rdx
	movq	8(%rdi), %rax
	movq	$1, (%rsi)
	testq	%rdx, %rdx
	jle	.L5
	leaq	(%rax,%rdx,8), %rcx
	movl	$1, %edx
	.p2align 4,,10
	.p2align 3
.L8:
	imulq	(%rax), %rdx
	addq	$8, %rax
	movq	%rdx, (%rsi)
	cmpq	%rax, %rcx
	jne	.L8
.L5:
	ret
	.cfi_endproc
.LFE3:
	.size	_Z8combine3P7vec_recPl, .-_Z8combine3P7vec_recPl
	.p2align 4,,15
	.globl	_Z8combine4P7vec_recPl
	.type	_Z8combine4P7vec_recPl, @function
_Z8combine4P7vec_recPl:
.LFB4:
	.cfi_startproc
	movq	(%rdi), %rcx
	movq	8(%rdi), %rdi
	testq	%rcx, %rcx
	jle	.L15
	leaq	-1(%rcx), %rax
	cmpq	$3, %rax
	jbe	.L16
	movq	%rcx, %rdx
	movdqa	.LC0(%rip), %xmm0
	movq	%rdi, %rax
	shrq	%rdx
	salq	$4, %rdx
	addq	%rdi, %rdx
	.p2align 4,,10
	.p2align 3
.L13:
	movdqu	(%rax), %xmm2
	movdqa	%xmm0, %xmm1
	addq	$16, %rax
	psrlq	$32, %xmm1
	movdqa	%xmm2, %xmm4
	pmuludq	%xmm2, %xmm1
	movdqa	%xmm2, %xmm3
	psrlq	$32, %xmm4
	pmuludq	%xmm0, %xmm3
	pmuludq	%xmm4, %xmm0
	paddq	%xmm1, %xmm0
	psllq	$32, %xmm0
	paddq	%xmm3, %xmm0
	cmpq	%rdx, %rax
	jne	.L13
	movdqa	%xmm0, %xmm4
	movdqa	%xmm0, %xmm2
	movdqa	%xmm0, %xmm1
	movq	%rcx, %rdx
	psrldq	$8, %xmm4
	psrlq	$32, %xmm2
	andq	$-2, %rdx
	pmuludq	%xmm4, %xmm2
	pmuludq	%xmm4, %xmm1
	movdqa	%xmm4, %xmm3
	psrlq	$32, %xmm3
	pmuludq	%xmm3, %xmm0
	paddq	%xmm0, %xmm2
	psllq	$32, %xmm2
	paddq	%xmm2, %xmm1
	movq	%xmm1, %rax
	cmpq	%rdx, %rcx
	je	.L10
.L11:
	leaq	1(%rdx), %r9
	leaq	0(,%rdx,8), %r8
	imulq	(%rdi,%rdx,8), %rax
	cmpq	%r9, %rcx
	jle	.L10
	imulq	8(%rdi,%r8), %rax
	leaq	2(%rdx), %r9
	cmpq	%r9, %rcx
	jle	.L10
	addq	$3, %rdx
	imulq	16(%rdi,%r8), %rax
	cmpq	%rdx, %rcx
	jle	.L10
	imulq	24(%rdi,%r8), %rax
.L10:
	movq	%rax, (%rsi)
	ret
	.p2align 4,,10
	.p2align 3
.L15:
	movl	$1, %eax
	movq	%rax, (%rsi)
	ret
.L16:
	movl	$1, %eax
	xorl	%edx, %edx
	jmp	.L11
	.cfi_endproc
.LFE4:
	.size	_Z8combine4P7vec_recPl, .-_Z8combine4P7vec_recPl
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB5:
	.cfi_startproc
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE5:
	.size	main, .-main
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.quad	1
	.quad	1
	.ident	"GCC: (GNU) 8.5.0 20210514 (Red Hat 8.5.0-4)"
	.section	.note.GNU-stack,"",@progbits
