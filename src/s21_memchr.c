#include "s21_string.h"
void *s21_memchr(const void *str, int c, s21_size_t n) {
  void *res = s21_NULL;
  const unsigned char *memchrStr =
      (const unsigned char *)str;  // unsigned char - это тип данных
                                   // представляет собой беззнаковый символ,
  // который обычно занимает 1 байт памяти.
  const unsigned char symbol = (const unsigned char)c;
  for (s21_size_t i = 0; i < n; i++) {
    if (memchrStr[i] == symbol) {
      res = (void *)(memchrStr + i);  // перемещаем указатель на найденый
                                      // элемент
      // надо сразу остановить работу функции чтобы она перестала дальше искать
      // символы
      //  при первом вхождении сразу же убираем

      break;
    }
  }
  return res;
}