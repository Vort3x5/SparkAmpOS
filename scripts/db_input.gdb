target remote localhost:1234
# symbol-file bin/SparkAmpOS.bin
b *0x100000
c
set disassembly-flavor intel
lay asm
