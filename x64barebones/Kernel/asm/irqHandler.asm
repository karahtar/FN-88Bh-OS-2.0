GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq11Handler
GLOBAL _sti
GLOBAL _cli
GLOBAL _setPicMaster
GLOBAL _setPicSlave


EXTERN irqDispatcher


%macro pushaq 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popaq 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandler 1

	pushaq

	mov rdi, %1
	call irqDispatcher
	
	mov al, 20h 										; EOI
	out 20h, al
	
	popaq

	iretq
%endmacro

%macro irqSlaveHandler 1
	pushaq

	mov rdi, %1
	call irqDispatcher
	
	mov al, 20h ; EOI

	out 0A0h, al ;EOI al PIC esclavo
	out 20h, al  ;EOI al PIC primario
	
	popaq

	iretq
%endmacro


section .text


; Timer Tick interrupt
_irq00Handler:
	irqHandler 0

; Keyboard interrupt
_irq01Handler:
	irqHandler 1

; PIC interrupt for priority 				LA USAMOS??
;_irq02Handler:
;	irqHandler 2

; RTL8139 interrupt
_irq11Handler:
	irqSlaveHandler 11

_sti:
	sti
	ret

_cli:
	cli
	ret


_setPicMaster:
	push rbp
	mov rbp, rsp
	
	mov rax, rdi
	out 21h, al

	mov rsp, rbp
	pop rbp
	ret

_setPicSlave:
	push rbp
	mov rbp, rsp
	
	mov rax, rdi
	out 0A1h, al

	mov rsp, rbp
	pop rbp
	ret
