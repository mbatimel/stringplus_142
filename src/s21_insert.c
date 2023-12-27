#include "s21_string.h"
void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *res = s21_NULL;
  s21_size_t j = 0, l = 0;
  s21_size_t len = s21_strlen(str) + s21_strlen(src) + 1;
  if (str && src && start_index <= s21_strlen(src)) {
    res = (char *)calloc(len, sizeof(char));
  }

  if (res) {
    for (s21_size_t i = 0; i < len; i++) {
      if (i >= start_index && i < s21_strlen(str) + start_index) {
        res[i] = str[j];
        j++;
      } else {
        res[i] = src[l];
        l++;
      }
    }
  }
  return (void *)res;
}