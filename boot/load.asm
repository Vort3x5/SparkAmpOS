; asmsyntax=fasm

use16
org 1000h
mov sp, 1000h

jmp Next

include 'print.inc'

Next:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax

	Print snd_loaded_msg 

	jmp A20Enable

include 'a20.inc'

A20Enable:
	call CheckA20
	cmp ax, 1
	je A20Done

	call BIOS_A20
	call CheckA20
	cmp ax, 1
	je A20Done
	Print a20_bios_failed_msg

	call KbA20
	call CheckA20
	cmp ax, 1
	je A20Done
	Print a20_keyboard_failed_msg

	call FastA20
	call CheckA20
	cmp ax, 1
	je A20Done
	Print a20_fast_failed_msg

	Print a20_failed_to_enable_msg

A20Done:
	Print a20_enabled_msg

GDT:
	dw (GDT_End - GDT_Start - 1)
	dd GDT_Start
 
ProtectedMode:
	cli
	lgdt [GDT]
	Print gdt_loaded_msg
	
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp 08h:JumpKernel

use32
JumpKernel:

	mov ax, 10h

	mov ds, ax
	mov fs, ax
	mov gs, ax
	mov es, ax
	mov ss, ax

	mov esp, 09000h
	mov ebp, esp
	std

	jmp 08h:2000h

	hlt

include 'gdt.inc'

snd_loaded_msg db 'Second Stage Loaded!', 0

a20_bios_failed_msg db 'BIOS Method Failed to Enable A20!', 0
a20_keyboard_failed_msg db 'Keyboard Controller Method Failed to Enable A20!', 0
a20_fast_failed_msg db 'Fast Method Failed to Enable A20!', 0
a20_failed_to_enable_msg db 'Failed To Enable A20!', 0

a20_enabled_msg db 'A20 Enabled!', 0

kernel_loaded_msg db 'Kernel Loaded!', 0
gdt_loaded_msg db 'GDT Loaded!', 0

times 2048 - ($ - $$) db 0
