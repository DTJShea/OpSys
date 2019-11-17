;kernel.asm
;Michael Black, 2007

;kernel.asm contains assembly functions that you can use in your kernel

	.global _putInMemory
	.global_interupt
	.global_makeInterrupt21
	.global _launchProgram
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


;this is called to start a program that is loaded into memory
;void launchProgram(int segment)
_launchProgram:
	mov bp,sp
	mov bx,[bp+2]	;get the segment into bx

	mov ax,cs	;modify the jmp below to jump to our segment
	mov ds,ax	;this is self-modifying code
	mov si,#jump
	mov [si+3],bx	;change the first 0000 to the segment

	mov ds,bx	;set up the segment registers
	mov ss,bx
	mov es,bx

	mov sp,#0xfff0	;set up the stack pointer
	mov bp,#0xfff0

jump:	jmp #0x0000:0x0000	;and start running (the first 0000 is changed above)

