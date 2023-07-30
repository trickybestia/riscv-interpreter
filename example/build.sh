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

mkdir -p "$OBJECTSDIR"

echo "*" > "$BUILDDIR/.gitignore"

BEAR="/usr/bin/bear"
CLANG="/usr/bin/clang"
LD="/usr/bin/ld.lld"
OBJCOPY="/usr/bin/llvm-objcopy"

CFLAGS="$CFLAGS --target=riscv32 -march=rv32im -nostdlib -std=c17 -Os"
LDFLAGS="$LDFLAGS -Tlink.ld"

COMPILE="$BEAR --append --output $BUILDDIR/compile_commands.json -- $CLANG $CFLAGS"

OBJECTS=()

for SOURCE in "${SOURCES[@]}"; do
  OBJECT="$OBJECTSDIR/$SOURCE.o"

  OBJECTS+=("$OBJECT")

  $COMPILE "src/$SOURCE" -c -o "$OBJECT"
done

$LD $LDFLAGS -o "$OBJECTSDIR/example.o" "${OBJECTS[@]}"

$OBJCOPY -O binary "$OBJECTSDIR/example.o" "$BUILDDIR/example.bin"
