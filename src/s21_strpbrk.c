#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  const char *tmp;
  int s1, s2;
  while ((s1 = *str1++) != 0) {
    for (tmp = str2; (s2 = *tmp++) != '\0';) {
      if (s2 == s1) {
        return (char *)(str1 - 1);
      }
    }
  }

  return s21_NULL;
}