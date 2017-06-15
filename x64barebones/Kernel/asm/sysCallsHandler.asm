; calls sysCallDispatcher.c with parameters

GLOBAL _sysCallHandler

EXTERN sysCallDispatcher


section .text

_sysCallHandler:

	push rbp
	mov rbp, rsp

	mov rdi, rax					; _sysCallHandler receives parameters in rax, rbx, rcx, rdx
	mov rsi, rbx					; parameters are copied to rdi, rsi, rdx, rcx to call sysCallDispatcher
	mov [backup], rdx
	mov rdx, rcx
	mov rcx, [backup]

	call sysCallDispatcher

	mov rsp, rbp
	pop rbp
	iretq

section .bss

	backup resq 1	;reserves 64-bit space for quad word

;		-rdi: syscall
;		-rsi: the file descriptor.
;		-rdx: the pointer to the buffer.
;		-rcx: the length of the message going to be read.

