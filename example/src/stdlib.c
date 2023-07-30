#include "stdlib.h"

int memcmp(const void *ptr1, const void *ptr2, size_t num) {
  for (size_t i = 0; i != num; i++) {
    unsigned char b1 = ((unsigned char *)ptr1)[i],
                  b2 = ((unsigned char *)ptr2)[i];

    if (b1 < b2)
      return -1;
    else if (b1 > b2)
      return 1;
  }

  return 0;
}

void *memset(void *dest, int ch, size_t count) {
  for (size_t i = 0; i != count; i++) {
    ((unsigned char *)dest)[i] = ch;
  }

  return dest;
}

void *memcpy(void *dest, const void *src, size_t count) {
  for (size_t i = 0; i != count; i++) {
    ((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
  }

  return dest;
}

void *memmove(void *dest, const void *src, size_t count) {
  if (dest == src)
    return dest;

  if (dest > src) {
    for (size_t i = count - 1; i + 1 != 0; i--) {
      ((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
    }
  } else {
    memcpy(dest, src, count);
  }

  return dest;
}
