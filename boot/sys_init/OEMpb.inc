; asmsyntax=fasm

bpbBytesPerSector  		dw 512
bpbSectorsPerCluster 	db 1
bpbReservedSectors 		dw 1
bpbNumberOfFATs 	    db 2
bpbRootEntries 	    	dw 224
bpbTotalSectors 	    dw 2880
bpbMedia 	            db 0xF0
bpbSectorsPerFAT 	    dw 9
bpbSectorsPerTrack 		dw 18
bpbHeadsPerCylinder 	dw 2
bpbHiddenSectors 	    dd 0
bpbTotalSectorsBig     	dd 0
bsDriveNumber 	        db 0
bsUnused 	            db 0
bsExtBootSignature 		db 0x29
bsSerialNumber	        dd 0xa0a1a2a3
bsVolumeLabel 	        db "MOS FLOPPY "
bsFileSystem 	        db "FAT12   "
