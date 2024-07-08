target remote localhost:1234
# symbol-file bin/SparkAmpOS.bin
b *0x100370
b *0x1003ab
c
set disassembly-flavor intel
lay asm
