GLOBAL _readKey


section .text
;
;_readKey
;	push rbp
;	mov rbp, rsp
	
;_readKey2
;	mov rax, 0
;	in	al, 64h
;	cmp	al, 01h		; 1 means there's input
;	je	_readInput
;	jmp	_readKey2

;_readInput
;	mov rax, 0
;	in	al, 60h		; grab input
;	jmp _exit		; return it (in rax registry)

;_exit
;	mov rsp, rbp
;	pop rbp
;	ret

_readKey:
	push rbp
	mov rbp, rsp
	in al, 60h
	mov rdi, rax
	mov rsp, rbp
	pop rbp
	ret