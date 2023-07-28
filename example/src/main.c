#include <stdbool.h>
#include <stdint.h>

#include "syscall.h"

void putchar(char c) { syscall(SYSCALL_PUTCHAR, c); }

void printBin(unsigned int n) {
  bool digits[32];

  for (int i = 0; i != 10; i++) {
    digits[31 - i] = n & 1;

    n >>= 1;
  }

  for (int i = 0; i != 32; i++)
    putchar('0' + digits[i]);

  putchar('\n');
}

int main() {
  for (int i = 0; i != 100; i++)
    printBin(i);

  return 0;
}
