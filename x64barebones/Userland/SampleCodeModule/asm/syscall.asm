;communicates with sysCallHandler in Kernel

GLOBAL _sysCall

section .text

_sysCall:
	push rbp
	mov rbp, rsp

	mov rax, rdi				;_sysCall parameters are rdi, rsi, rdx, rcx
	mov rbx, rsi				;parameters are copied to rax, rbx, rcx, rdx to do int 80h

	int 80h

	mov rsp,rbp
	pop rbp
	ret


;		-rdi: syscall
;		-rsi: the file descriptor.
;		-rdx: the length of the message going to be read.
;		-rcx: the pointer to the buffer.