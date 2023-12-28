target remote localhost:1234
# symbol-file SparkAmpOS
symbol-file bin/SparkAmpOS.bin
# b *0x100000
# b *0x100148
b *0x7c00
# b Main
c
lay asm
