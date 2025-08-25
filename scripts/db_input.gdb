target remote localhost:1234
symbol-file bin/SparkAmpOS.elf
b Malloc
c
set disassembly-flavor intel
lay src
