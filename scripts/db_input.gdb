target remote localhost:1234
# symbol-file bin/SparkAmpOS.bin
b *0x100000
# b *0x100921
c
set disassembly-flavor intel
lay asm
