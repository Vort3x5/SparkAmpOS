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
bpb_dir_entries_count		dw 224
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
ebr_volume_label			db "BlazeFS"
ebr_fs_type					db "FAT16 	"

start:
	call regset
	call reset_disk
	call print_startup

	cli
	hlt

regset: ; Setup 16-bit segment registers and stack
	cli

	mov ax, cs
	mov ds, ax
	mov es, ax

	sti
	ret

reset_disk:
	push ax
	mov dl, 0x0
	xor ax, ax
	int 0x13
	pop ax
	ret

print_startup:
	push si
	mov si, start_msg
	mov ah, 0x0e
.print:
	lodsb
	int 0x10
	cmp al, 0
	jne .print
	pop si
	ret

start_msg db 'Initializing!', 0

times 510 - ($ - $$) db 0
dw 0xaa55
