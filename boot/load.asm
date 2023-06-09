 ; asmsyntax=fasm

mov sp, 1000h

mov si, next_msg
mov ah, 0eh
call print_line
jmp load_kernel

bpbBytesPerSector  	dw 512
bpbSectorsPerCluster 	db 1
bpbReservedSectors 	dw 1
bpbNumberOfFATs 	db 2
bpbRootEntries 	dw 224
bpbSectorsPerFAT 	dw 9
bpbSectorsPerTrack 	dw 18
bpbHeadsPerCylinder 	dw 2

include 'bios_fns.inc'

absoluteSector db 0
absoluteHead   db 0
absoluteTrack  db 0
bsDriveNumber  db 0

datasector dw 0000h
cluster dw 0

next_msg db 'cont!', 0
image_name db 'SparkAmpOS '

lba_to_chs:
	xor dx, dx
	div word [bpbSectorsPerTrack]
	inc dl
	mov byte [absoluteSector], dl
	xor dx, dx
	div word [bpbHeadsPerCylinder]
	mov byte [absoluteHead], dl
	mov byte [absoluteTrack], al
	ret

read_sectors:
 	push ax
 	push bx
 	push cx
	call lba_to_chs
	mov ah, 02h
	mov al, 01h
	mov ch, byte [absoluteTrack]
	mov cl, byte [absoluteSector]
	mov dh, byte [absoluteHead]
	mov dl, byte [bsDriveNumber]
	int 13h

 	pop cx
 	pop bx
 	pop ax
	add bx, word [bpbBytesPerSector]
	inc ax
	loop read_sectors
	ret

load_kernel:
; count root size
	xor cx, cx
	xor dx, dx
	mov ax, 0020h
	mul word [bpbRootEntries]
	div word [bpbBytesPerSector]
	xchg ax, cx
	
; root dir location into ax
	mov al, byte [bpbNumberOfFATs]
	mul word [bpbSectorsPerFAT]
	add ax, [bpbReservedSectors]
	mov word [datasector], ax
	mov word [datasector], cx

; read root dir into memory
	mov bx, 0200h
	call read_sectors

; find bin image
	mov cx, [bpbRootEntries]
	mov di, 0200h

	.loop_root:
		push cx
		mov	cx, 0x000b
		mov si, image_name
		push di

		rep cmpsb ; check for match
		pop di
		je .load_fat
		pop cx
		add di, 0020h
		loop .loop_root

	.load_fat:
	; save starting cluster
		mov dx, word [di + 001ah]
		mov word [cluster], dx

	; store fat size in cx
		xor ax, ax
		mov al, byte [bpbNumberOfFATs]
		mul word [bpbSectorsPerFAT]
		mov cx, ax

	; store fat location in ax
		mov ax, word [bpbReservedSectors]

	; read fat into memory
		mov bx, 0200h
		call read_sectors

	; read image into memory
	mov ax, 0050h
	mov es, ax
	mov bx, 0000h
	push bx

	.load_image:
		mov ax, word [cluster]
		pop bx
	; Cluster to LBA
		sub ax, 0002h
		xor cx, cx
		mov cl, byte [bpbSectorsPerCluster]
		mul cx
		add ax, word [datasector]

		xor cx, cx
		mov cl, byte [bpbSectorsPerCluster]
		call read_sectors
		push bx

	; next cluster
		mov ax, word [cluster]
		mov cx, ax
		mov dx, ax
		shr dx, 0001h
		add cx, dx
		mov bx, 0200h
		add bx, cx
		mov dx, word [bx]
		test ax, 0001h
		jnz .odd_cluster

	.even_cluster:
		and dx, 0000111111111111b
		jmp .done

	.odd_cluster:
		shr dx, 0004h

	.done:
		mov word [cluster], dx
		cmp dx, 0ff0h
		jb .load_image

gdt:
	dw (gdt_end - gdt_start - 1)
	dd gdt_start
 
protected_mode:
	cli
	lgdt [gdt]
	
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp 08h:jump_kernel

use32
jump_kernel:
	mov ax, 10h
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, 90000h
	jmp 08h:0050h

gdt_start:
; null
	dq 0h
; code
	dw 0ffffh
	dw 0h
	db 0h
	db 10011010b
	db 11001111b
	db 0
; data
	dw 0ffffh
	dw 0h
	db 0h
	db 10010010b
	db 11001111b
	db 0
gdt_end:

times 1024 - ($ - $$) db 0
