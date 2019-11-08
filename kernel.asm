;kernel.asm
;Michael Black, 2007

;kernel.asm contains assembly functions that you can use in your kernel

	.global _putInMemory
	.global_interupt
	.global_makeInterrupt21
 	.extern_handleInterrupt21

;void putInMemory (int segment, int address, char character)
_putInMemory:
	push bp
	mov bp,sp
	push ds
	mov ax,[bp+4]
	mov si,[bp+6]
	mov cl,[bp+8]
	mov ds,ax
	mov [si],cl
	pop ds
	pop bp
	ret

_interrupt:
	push bp
	mov bp,sp
	mov ax,[bp+4]
	push ds
	mov bx,cs
	mov ds,bx
	mov si,#intr
	mov [si+1],al
	pop ds
	mov ax,[bp+6]
	ovm bx,[bp+8]
	mov cx,[bp+10]
	mov dx,[bp+12]

intr: int#0x00
	mov ah,#0
	pop bp
	ret

_makeInterrupt21:
	mov dx,#_interruptServiceRoutine
	push ds
	mov ax,#0
	mov ds,ax
	mov si,#0x84
	mov ax,cs
	mov [si+2],ax
	mov [si],dx
	pop ds
	ret

_interrupt21ServiceRoutine:
 	push dx
  	push cx
 	push bx
	push ax
	call_handleInterrupt21
	pop ax
	pop bx
	pop cx
	pop dx

	iret
