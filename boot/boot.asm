; asmsyntax=fasm

org 0x7c00

use16

jmp start
nop

bpb_oem 					db 'SparkAmpOS'
bpb_bytes_per_sector 	 	dw 512
bpb_sectors_per_cluster 	db 1
bpb_reserved_sectors		dw 1
bpb_fat_count				db 2
bpb_root_size				dw 224
bpb_total_sectors			dw 0x2880
bpb_media_descriptor_type	db 0xf0
bpb_sectors_per_fat			dw 9
bpb_sectors_per_track		dw 18
bpb_heads					dw 2
bpb_hidden_sectors			dd 0
bpb_large_sector_count		dd 0

ebr_drive_number			db 0
db 0
ebr_signature				db 0x29
ebr_volume_id				dd 0x12345678
ebr_volume_label			db "BlazeFS	"
ebr_fs_type					db "FAT12 	"

start:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7c00

	push es
	push word print_startup
	retf

print_startup:
	mov si, start_msg
	mov ah, 0x0e
	call print
	jmp root_compute

print:
	lodsb
	int 0x10
	cmp al, 0
	jne print
.new_line:
	mov al, 13
	int 0x10
	mov al, 10
	int 0x10
	ret

root_compute:
	mov ax, 32
	xor dx, dx
	mul word ptr bpb_root_size
	div word ptr bpb_bytes_per_sector
	mov cx, ax
	mov word [root_sectors_count], cx

	xor ax, ax
	mov al, byte ptr bpb_fat_count
	mov al, byte ptr bpb_sectors_per_fat
	mul bx
	add ax, word ptr bpb_hidden_sectors
	adc ax, word ptr bpb_hidden_sectors + 2
	add ax, word ptr bpb_reserved_sectors
	mov word [root_start], ax
	jmp read_all_sectors

read_sector:
	xor cx, cx

	push ax
	push cx
	push bx

	mov bx, bpb_sectors_per_track
	xor dx, dx
	div bx
	inc dx
	mov cl, dl
	mov bx, bpb_heads
	xor dx, dx
	div bx
	mov ch, al
	xchg dl, dh

	mov ax, 0x0201
	movzx bx, byte [ebr_drive_number]
	mov dl, bl
	pop bx
	int 0x13
	jc read_sector_err

	pop cx
	pop ax
	ret

read_all_sectors:
	push cx
	push ax
	xor bx, bx
	call read_sector
check_entry:
	mov cx, 11
	mov di, bx
	lea si, [snd_stage_bin]
	repz cmpsb
	je found_bin
	add bx, word ptr 32
	cmp bx, word ptr bpb_bytes_per_sector
	jne check_entry
	pop ax
	inc ax
	pop cx
	loopnz read_all_sectors
	jmp boot_failed

read_sector_err:
	pop cx
	inc cx
	cmp cx, word ptr 4
	je boot_failed

	xor ax, ax
	int 0x13
	
	pop ax
	jmp read_sector

found_bin:
	mov ax, [es:bx+0x1a]
	mov [file_start], ax
	jmp halt

boot_failed:
	mov si, boot_failed_msg
	mov ah, 0x0e
	call print

halt:
	cli
	hlt

start_msg db 'Initializing!', 0
boot_failed_msg db 'Boot Process Failed!', 0

snd_stage_bin db 'snd  bin', 0
root_sectors_count dw 0
root_start dw 0
file_start dw 0

times 510 - ($ - $$) db 0
dw 0xaa55
