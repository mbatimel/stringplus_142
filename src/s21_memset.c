#include "s21_string.h"
void *s21_memset(void *str, int c, s21_size_t n) {
  char *memchrStr = (char *)str;
  const char symbol = (const char)c;
  for (s21_size_t i = 0; i < n; i++) {
    memchrStr[i] = symbol;
  }
  return (void *)memchrStr;
}