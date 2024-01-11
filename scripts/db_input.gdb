target remote localhost:1234
# symbol-file bin/SparkAmpOS
# symbol-file bin/SparkAmpOS.bin
b *0x100000
# b Main
c
lay asm
