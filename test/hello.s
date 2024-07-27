	.file	"hello.c"
	.text
	.globl	message
	.data
	.align 4
	.type	message, @object
	.size	message, 16
message:
	.string	"hello world!!!\n"
	.globl	buf
	.bss
	.align 32
	.type	buf, @object
	.size	buf, 1024
buf:
	.zero	1024
	.section	.rodata
.LC0:
	.string	"%s"
	.text
	.globl	main
	.type	main, @function
main:
	pushl	$message
	pushl	$.LC0
	call	printf
	addl	$8, %esp
	movl	$0, %eax
	ret
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
