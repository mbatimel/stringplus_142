#include "s21_string.h"
void *s21_to_upper(const char *str) {
  char *res = s21_NULL;
  if (str) {
    res = (char *)calloc(s21_strlen(str) + 1, sizeof(char));
  }
  if (res) {
    int i = 0;
    s21_strncpy(res, str, s21_strlen(str) + 1);
    while (str[i] != '\0') {
      if (str[i] >= 97 && str[i] <= 122) {
        res[i] -= 32;
      }
      i++;
    }
  }
  return (void *)res;
}