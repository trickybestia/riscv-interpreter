#!/usr/bin/sh

set -e

SOURCES=(
_start.s
syscall.s
stdlib.c

main.c
)

BUILDDIR="build"
OBJECTSDIR="$BUILDDIR/objects"

BEAR="bear"
CLANG="clang"
LD="ld.lld"
OBJCOPY="llvm-objcopy"

CFLAGS="$CFLAGS --target=riscv32 -march=rv32im -nostdlib -std=c17 -Os"
LDFLAGS="$LDFLAGS -Tlink.ld"

COMPILE="$BEAR --append --output $BUILDDIR/compile_commands.json -- $CLANG $CFLAGS"

mkdir -p "$OBJECTSDIR"

echo "*" > "$BUILDDIR/.gitignore"

OBJECTS=()

for SOURCE in "${SOURCES[@]}"; do
  OBJECT="$OBJECTSDIR/$SOURCE.o"

  OBJECTS+=("$OBJECT")

  $COMPILE "src/$SOURCE" -c -o "$OBJECT"
done

$LD $LDFLAGS -o "$OBJECTSDIR/example.o" "${OBJECTS[@]}"

$OBJCOPY -O binary "$OBJECTSDIR/example.o" "$BUILDDIR/example.bin"
