target remote localhost:1234
symbol-file bin/SparkAmpOS
b Main
c
set disassembly-flavor intel
lay src
