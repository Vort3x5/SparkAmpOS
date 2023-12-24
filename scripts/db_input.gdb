target remote localhost:1234
# symbol-file SparkAmpOS
# b *0x100033
# symbol-file bin/SparkAmpOS.bin
b *0x100000
b *0x100148
c
lay asm
