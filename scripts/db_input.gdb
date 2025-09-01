target remote localhost:1234
symbol-file bin/SparkAmpOS.elf
b Print
c
set disassembly-flavor intel
lay src
