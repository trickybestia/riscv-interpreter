#!/usr/bin/sh

mkdir -p build

echo "*" > build/.gitignore

CLANG="/usr/bin/clang"
LD="/usr/bin/ld.lld"
OBJCOPY="/usr/bin/llvm-objcopy"

CFLAGS="$CFLAGS --target=riscv32 -march=rv32i -nostdlib"
LDFLAGS="$LDFLAGS -Tlink.ld"

BUILDDIR="build"

$CLANG $CFLAGS src/main.c -S -o "$BUILDDIR"/main.s
$CLANG $CFLAGS src/_start.s -c -o "$BUILDDIR"/_start.o
$CLANG $CFLAGS src/syscall.s -c -o "$BUILDDIR"/syscall.o
$CLANG $CFLAGS src/main.c -c -o "$BUILDDIR"/main.o

$LD $LDFLAGS -o "$BUILDDIR"/example.o "$BUILDDIR"/_start.o "$BUILDDIR"/syscall.o "$BUILDDIR"/main.o

$OBJCOPY -O binary "$BUILDDIR"/example.o "$BUILDDIR"/example.bin
