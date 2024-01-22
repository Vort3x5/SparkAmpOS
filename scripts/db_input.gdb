target remote localhost:1234
# symbol-file bin/SparkAmpOS
symbol-file bin/SparkAmpOS.bin
b _Start
c
lay asm
