#include "stdlib.h"
#include "syscall.h"

void putchar(char c) { syscall(SYSCALL_PUTCHAR, c); }

void printNumber(uint32_t n) {
  char digits[10] = {0};

  for (int i = 0; i != 10 && n != 0; i++) {
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
  printNumber(sizeof(size_t) * 100);

  return 0;
}
