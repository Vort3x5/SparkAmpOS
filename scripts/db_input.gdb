target remote localhost:1234
symbol-file bin/SparkAmpOS.elf
b GenerateSineWave
c
set disassembly-flavor intel
lay src
