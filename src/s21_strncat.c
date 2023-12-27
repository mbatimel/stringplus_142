#include "s21_string.h"
char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *memchrStr = dest;
  const char *symbol = (const char *)src;
  s21_size_t len = s21_strlen(memchrStr);
  for (s21_size_t i = 0; i < n; i++) {
    memchrStr[len + i] = symbol[i];
  }
  return memchrStr;
}