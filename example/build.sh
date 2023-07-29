#!/usr/bin/sh

set -e

mkdir -p build

echo "*" > build/.gitignore

BUILDDIR="build"

BEAR="/usr/bin/bear"
CLANG="/usr/bin/clang"
LD="/usr/bin/ld.lld"
OBJCOPY="/usr/bin/llvm-objcopy"

CFLAGS="$CFLAGS --target=riscv32 -march=rv32i -nostdlib -std=c17 -Os"
LDFLAGS="$LDFLAGS -Tlink.ld"

COMPILE="$BEAR --append --output $BUILDDIR/compile_commands.json -- $CLANG $CFLAGS"

$COMPILE src/_start.s -c -o "$BUILDDIR"/_start.o
$COMPILE src/syscall.s -c -o "$BUILDDIR"/syscall.o
$COMPILE src/main.c -c -o "$BUILDDIR"/main.o

$LD $LDFLAGS -o "$BUILDDIR"/example.o "$BUILDDIR"/_start.o "$BUILDDIR"/syscall.o "$BUILDDIR"/main.o

$OBJCOPY -O binary "$BUILDDIR"/example.o "$BUILDDIR"/example.bin
