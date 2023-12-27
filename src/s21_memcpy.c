#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  unsigned char *csrc = (unsigned char *)src;
  unsigned char *cdest = (unsigned char *)dest;
  if (n != 0 || dest != src) {
    for (s21_size_t i = 0; i < n; i++) {
      cdest[i] = csrc[i];
    }
  }
  return dest;
}