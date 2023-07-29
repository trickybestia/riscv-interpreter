#include <stdbool.h>
#include <stdint.h>

#include "syscall.h"

void putchar(char c) { syscall(SYSCALL_PUTCHAR, c); }

void printNumber(uint32_t n) {
  char digits[10];

  for (int i = 0; i != 10; i++) {
    if (n == 0) {
      digits[9 - i] = 0;

      continue;
    }

    digits[9 - i] = '0' + n % 10;
    n /= 10;
  }

  for (int i = 0; i != 10; i++) {
    if (digits[i] == 0)
      continue;

    putchar(digits[i]);
  }

  putchar('\n');
}

int main() {
  for (uint32_t i = 2147483648; i != 2147483658; i++)
    printNumber(i);

  return 0;
}
