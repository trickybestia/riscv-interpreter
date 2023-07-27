const char *STRING = "1234567890";

void putchar(char c) {
  __asm__ volatile("mv a0, %0\n\t"
                   "ecall\n" ::"r"(c)
                   : "a0");
}

void __attribute__((__section__(".entry"))) entry() {
  for (int i = 0; i != 10; i++) {
    putchar(STRING[i]);
  }

  __asm__ volatile("ebreak");
}
