target remote localhost:1234
# symbol-file bin/SparkAmpOS.bin
b *0x101390
c
set disassembly-flavor intel
lay asm
