#include <stdint.h>

const uint32_t SYSCALL_PUTCHAR = 1;

extern int32_t syscall(uint32_t number, int32_t arg1);
