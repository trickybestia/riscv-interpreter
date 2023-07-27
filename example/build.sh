#!/usr/bin/sh

mkdir -p build

echo "*" > build/.gitignore

CFLAGS="--target=riscv32 -march=rv32i -nostdlib -Os"
CLANG="/usr/bin/clang"
LD="/usr/bin/ld.lld"
LDFLAGS="-Tlink.ld"

$CLANG $CFLAGS main.c -S -o build/main.s
$CLANG $CFLAGS main.c -c -o build/main.o

$LD $LDFLAGS build/main.o -o build/example.o

llvm-objcopy -O binary build/example.o build/example.bin
