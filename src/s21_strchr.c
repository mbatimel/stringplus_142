#include "s21_string.h"

char *s21_strchr(const char *str, int c) {
  const char *result = s21_NULL;
  int x = s21_strlen(str) + 1;
  for (int i = 0; i < x; i++) {
    if (str[i] == c) {
      result = (str + i);
      break;
    }
  }
  return (char *)result;
}