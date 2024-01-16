target remote localhost:1234
# symbol-file bin/SparkAmpOS
# symbol-file bin/SparkAmpOS.bin
# b *0x100000
# b *0x100210
# b InstallTimer
b *0x7c00
b *0x7c61
b *0x7c6d
c
lay asm
