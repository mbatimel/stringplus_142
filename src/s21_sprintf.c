#include "s21_string.h"

int s21_sprintf(char *str, const char *format, ...) {
  va_list dop_values = {0};
  s21_Format textformat = {0};
  va_start(dop_values, format);
  s21_size_t i = 0;
  str = s21_strcpy(str, "");
  while (*format) {
    textformat.flag_minus = textformat.flag_plus = textformat.flag_space =
        textformat.flag_sharp = textformat.flag_zero = textformat.point = false;
    textformat.precision = -1;
    textformat.sign = textformat.length = textformat.width =
        textformat.specifier = 0;
    switch (*format) {
      case '%':
        format++;
        if (is_flag(*format)) {  // Флаги
          while (is_flag(*format)) {
            s21_check_flags(format++, &textformat);
          }
        }
        if (is_digit(*format) || *format == '*') {  // ширина
          format = s21_str_to_int(&textformat.width, format++, &dop_values);
          if (textformat.width < 0) {
            textformat.flag_minus = true;
            textformat.width *= -1;
          }
        }
        if (*format == '.') {
          textformat.point = true;
          textformat.precision = 0;
          format++;
          if (*format == '*' || is_digit(*format)) {  // Точность
            format =
                s21_str_to_int(&textformat.precision, format++, &dop_values);
          } else {
            textformat.precision = -1;
          }
        }

        if (is_length(*format)) {  // длина
          while (is_length(*format)) {
            textformat.length = *(format++);
          }
        }
        if (is_specifier(*format)) {  // спецификатор
          textformat.specifier = *(format++);
        }

        s21_default_precision(&textformat);

        str = treatment(str, &textformat, &dop_values, &i);
        break;
      default:

        str[i++] = *(format++);
        str[i] = '\0';

        break;
    }
  }
  va_end(dop_values);
  return (int)s21_strlen(str);
}

/*Работа спецификаторов*/
char *treatment(char *str, s21_Format *textformat, va_list *dop_values,
                s21_size_t *len) {
  char *str_res = s21_NULL;
  str_res = calloc(1024, sizeof(char));
  switch (textformat->specifier) {
    case 'c':
      s21_spec_c(str_res, dop_values, textformat);
      break;
    case 'f':
      s21_spec_f(str_res, dop_values, textformat);
      break;
    case 'e':
    case 'E':
      s21_spec_eE(str_res, dop_values, textformat);
      break;
    case 'g':
    case 'G':
      s21_spec_gG(str_res, dop_values, textformat);
      break;
    case 'i':
    case 'd':

      s21_spec_di(str_res, dop_values, textformat);
      break;
    case 'o':
      s21_o_spec(str_res, dop_values, textformat);
      break;
    case 's':
      s21_s_spec(str_res, dop_values, textformat);
      break;
    case 'u':
      s21_u_spec(str_res, dop_values, textformat);
      break;
    case 'x':
    case 'X':
      s21_xX_spec(str_res, dop_values, textformat);
      break;
    case 'p':
      s21_p_spec(str_res, dop_values, textformat);
      break;
    case 'n':
      s21_n_spec(len, dop_values);
      break;
    case '%':
      s21_prosent_spec(str_res, textformat);
      break;
  }
  s21_strcat(str, str_res);
  free(str_res);
  *len = s21_strlen(str);
  return str;
}
/*дополнительные условия для точности*/
void s21_default_precision(s21_Format *parameters) {
  if (parameters->point && parameters->precision <= 0) {
    parameters->precision = 0;
  }
  if (!parameters->point && s21_strchr("eEfgG", parameters->specifier)) {
    parameters->precision = 6;
  }
}
/*записываем точность в структуру*/
char *s21_str_to_int(int *number, const char *format, va_list *input) {
  char wth[128] = {0};
  char *num = NULL;
  int count = 0;
  while (is_digit(*format)) {
    wth[count++] = *(format)++;
  }
  num = wth;
  while (count > 0) {
    *number += ((*(num)++) - '0') * pow(10, --count);
  }
  if (*format == '*') {
    *number = va_arg(*input, int);
    format++;
  }
  return (char *)format;
}
/*проверка на заполнение флагов*/
void s21_check_flags(const char *str_res, s21_Format *textformat) {
  switch (*str_res) {
    case '-':
      textformat->flag_minus = true;
      break;
    case '+':
      textformat->flag_plus = true;
      break;
    case ' ':
      textformat->flag_space = true;
      break;
    case '#':
      textformat->flag_sharp = true;
      break;
    case '0':
      textformat->flag_zero = true;
      break;
  }
}
/*Работа флага # */
void s21_sharp_work(char *str_res, s21_Format *textformat) {
  int pos = 0;
  if (s21_strchr("oxX", textformat->specifier)) {
    if (textformat->specifier == 'x' || textformat->specifier == 'X') {
      s21_move_str_right(str_res);
    }
    if (textformat->specifier != 'o') {
      s21_move_str_right(str_res);
    }
    str_res[pos++] = '0';
    if (textformat->specifier != 'o') {
      str_res[pos] = textformat->specifier == 'x' ? 'x' : 'X';
    }
  }
  if (s21_strchr("eEf", textformat->specifier)) {
    if (!s21_strchr(str_res, '.')) {
      str_res[s21_strlen(str_res)] = '.';
    }
  }
}
/*двигаем значение направо*/
void s21_move_str_right(char *str_res) {
  int pos = (int)s21_strlen(str_res);
  str_res[pos + 1] = '\0';
  while (pos > 0) {
    str_res[pos] = str_res[pos - 1];
    pos--;
  }
}
/*работа параметров точности*/
void s21_work_precision(char *str_res, s21_Format *textformat) {
  char *str_precision = s21_NULL;
  str_precision = (char *)calloc(1024, sizeof(char));
  int pos = 0;
  if (str_precision != s21_NULL) {
    if (s21_strchr("diouxX", textformat->specifier)) {
      while (pos < textformat->precision - (int)s21_strlen(str_res)) {
        str_precision[pos++] = '0';
      }
      for (int i = 0; str_res[i] != '\0'; i++) {
        str_precision[pos++] = str_res[i];
      }
      s21_strcpy(str_res, str_precision);
    }
    free(str_precision);
  }
}
/*Выполнение работы флагов*/
void s21_work_flag(char *str_res, s21_Format *textformat) {
  int point = 0;
  if (textformat->sign < 0) {
    s21_move_str_right(str_res);
    str_res[point++] = '-';
    textformat->sign = 0;
  }
  if (textformat->flag_plus && textformat->sign != 0) {
    s21_move_str_right(str_res);
    if (textformat->sign > 0) {
      str_res[point] = '+';
    } else {
      str_res[point] = '-';
    }
    textformat->sign = 0;
  }
  if (textformat->flag_space && textformat->sign != 0) {
    s21_move_str_right(str_res);
    if (textformat->sign > 0) {
      str_res[point] = ' ';
    } else {
      str_res[point] = '-';
    }
    textformat->sign = 0;
  }
  if (textformat->flag_sharp) {
    s21_sharp_work(str_res, textformat);
  }
  if (textformat->flag_minus) {
    int position = (int)s21_strlen(str_res);
    while (position < textformat->width) {
      str_res[position++] = ' ';
    }
  }
}

/*Беззнаковое восьмеричное число */
void s21_o_spec(char *str_res, va_list *dop_values, s21_Format *textformat) {
  unsigned long int l_number = 0;
  unsigned short int h_number = 0;
  unsigned int number = 0;

  switch (textformat->length) {
    case 'h':
      h_number = (unsigned short int)va_arg(*dop_values, unsigned int);
      s21_ol_to_str(str_res, h_number, textformat);
      break;
    case 'l':
      l_number = (unsigned long int)va_arg(*dop_values, unsigned long int);
      s21_ol_to_str(str_res, l_number, textformat);
      break;
    default:
      number = va_arg(*dop_values, unsigned int);
      s21_ol_to_str(str_res, number, textformat);
      break;
  }
  s21_work_precision(str_res, textformat);
  s21_work_flag(str_res, textformat);
  s21_make_string_width(textformat, str_res);
}
/* Строка символов */
void s21_s_spec(char *str, va_list *dop_value, s21_Format *textformat) {
  char *string = s21_NULL;
  string = va_arg(*dop_value, char *);

  if (textformat->precision > -1)
    s21_strncat(str, string, textformat->precision);
  else
    s21_strcat(str, string);
  s21_work_precision(str, textformat);
  s21_work_flag(str, textformat);
  s21_make_string_width(textformat, str);
}
/*Беззнаковое десятичное целое число */
void s21_u_spec(char *str_res, va_list *dop_value, s21_Format *textformat) {
  unsigned long l_number = 0;
  unsigned short h_number = 0;
  unsigned int number = 0;

  switch (textformat->length) {
    case 'h':
      h_number = (unsigned short)va_arg(*dop_value, unsigned int);
      s21_ul_to_str(str_res, h_number, textformat);
      break;
    case 'l':
      l_number = (unsigned long)va_arg(*dop_value, unsigned long);
      s21_ul_to_str(str_res, l_number, textformat);
      break;
    default:
      number = va_arg(*dop_value, unsigned int);
      s21_ul_to_str(str_res, number, textformat);
      break;
  }
  s21_work_precision(str_res, textformat);
  s21_work_flag(str_res, textformat);
  s21_make_string_width(textformat, str_res);
}
/* Беззнаковое шестнадцатеричное целое число(X - заглавные буквы) */
void s21_xX_spec(char *str_res, va_list *dop_value, s21_Format *textformat) {
  unsigned long l_number = 0;
  unsigned short h_number = 0;
  unsigned int number = 0;

  switch (textformat->length) {
    case 'h':
      h_number = (unsigned short)va_arg(*dop_value, unsigned int);
      s21_xl_to_str(str_res, h_number, textformat);
      break;
    case 'l':
      l_number = (unsigned long)va_arg(*dop_value, unsigned long);
      s21_xl_to_str(str_res, l_number, textformat);
      break;
    default:
      number = va_arg(*dop_value, unsigned int);
      s21_xl_to_str(str_res, number, textformat);
      break;
  }
  s21_work_precision(str_res, textformat);
  s21_work_flag(str_res, textformat);
  s21_make_string_width(textformat, str_res);
}
/*Адрес указателя*/
void s21_p_spec(char *str_res, va_list *dop_value, s21_Format *textformat) {
  int width = textformat->width < 15 ? 15 : textformat->width;
  char *str_start = s21_NULL;
  str_start = str_res;
  int *p = va_arg(*dop_value, int *);
  char buf[4] = "x0";
  if (p == s21_NULL) {
    s21_strcpy(buf, "0x0");
  }
  for (int *j = p, k = 0; j && k < width;
       j = (void *)(((s21_size_t)j) >> 4), k++) {
    unsigned int dig = ((s21_size_t)j) % 0x10;
    if (dig < 10)
      *(str_start++) = (char)('0' + dig);
    else
      *(str_start++) = (char)('a' + (dig - 10));
  }
  *str_start = '\0';
  s21_strcat(str_res, buf);
  s21_reverse_writing(str_res);
  s21_work_precision(str_res, textformat);
  s21_work_flag(str_res, textformat);
  s21_make_string_width(textformat, str_res);
}
void s21_n_spec(s21_size_t const *str_len, va_list *dop_value) {
  int *p = va_arg(*dop_value, int *);
  *p = (int)*str_len;
}
void s21_prosent_spec(char *str, s21_Format *textformat) {
  s21_strcat(str, "%");
  s21_work_precision(str, textformat);
  s21_work_flag(str, textformat);
}

/* Функция обратной записи в строку */
void s21_reverse_writing(char *str) {
  char c = 0;
  s21_size_t length = s21_strlen(str);
  for (s21_size_t i = 0; i < length / 2; i++) {
    c = str[i];
    str[i] = str[length - 1 - i];
    str[length - 1 - i] = c;
  }
}
/*Конвертация в 8 систему исчисления*/
void s21_ol_to_str(char *str_res, unsigned long long number,
                   s21_Format *textformat) {
  char *point = s21_NULL;
  int flag = 1;

  point = str_res;  // указатель на конец строки

  while (flag) {
    unsigned int digit;
    if (number > 7) {
      digit = number % 8;
      number = (number - digit) / 8;
    } else {
      digit = number;
      flag = 0;
    }
    *(point++) = (char)(digit + '0');
  }

  if (textformat->flag_sharp) *(point++) = '0';
  *point = '\0';
  s21_reverse_writing(str_res);
}
/*Парсер обычного целочисленного числа без знака*/
void s21_ul_to_str(char *str_res, unsigned long long number,
                   s21_Format *textformat) {
  char *point = s21_NULL;
  int flag = 1;

  point = str_res;  // указатель на конец строки

  while (flag) {
    unsigned int digit;
    if (number >= 10) {
      digit = (int)fmod(number, 10);
      number = (number - digit) / 10;
    } else {
      digit = (int)number;
      flag = 0;
    }
    *(point++) = (char)(digit + '0');
  }
  if (textformat->precision == 0 && number == 0 && textformat->point)
    str_res[0] = '\0';
  *point = '\0';

  s21_reverse_writing(str_res);
}
/*парсер в 16 формат*/
void s21_xl_to_str(char *str_res, unsigned long long number,
                   s21_Format *textformat) {
  char *point = s21_NULL;
  int flag = 1, shift = 0;
  point = str_res;
  shift = textformat->specifier == 'x' ? 32 : 0;

  while (flag) {
    unsigned int digit;
    if (number > 15) {
      digit = number % 16;
      number = (number - digit) / 16;
    } else {
      digit = number;
      flag = 0;
    }
    if (digit < 10)
      *(point++) = (char)(digit + '0');
    else
      *(point++) = (char)(digit + 'A' + shift - 10);
  }
  *point = '\0';
  if (number == 0) textformat->flag_sharp = 0;
  s21_reverse_writing(str_res);
}
void s21_gG_to_str(char *str, long double number, s21_Format *parameters) {
  parameters->sign = number < 0 ? -1 : 1;
  number *= parameters->sign;
  int local_sign = parameters->sign;
  if ((number / pow(10, parameters->precision)) > 1 ||
      ((number * (pow(10, 4))) < 1 && (number * (pow(10, 21))) >= 1)) {
    if (number < 1 && parameters->precision <= 0) {
      parameters->precision = 4;
    } else {
      --parameters->precision;
    }
    s21_eE_to_str(str, number, parameters);
  } else {
    parameters->precision > 0 ? 1 : ++parameters->precision;
    s21_float_to_str(str, parameters, number);
    s21_delete_zero(parameters, str);
  }
  parameters->sign = local_sign;
}
void s21_spec_gG(char *str, va_list *dop_values, s21_Format *textformat) {
  long double l_number = 0;
  double d_number = 0;

  switch (textformat->length) {
    case 'L':
      l_number = va_arg(*dop_values, long double);
      s21_gG_to_str(str, l_number, textformat);

      break;

    case 'l':
    default:
      d_number = va_arg(*dop_values, double);
      s21_gG_to_str(str, d_number, textformat);
      break;
  }
  s21_work_precision(str, textformat);
  s21_work_flag(str, textformat);
  s21_make_string_width(textformat, str);
}

void treatment_for_zeroes(char *str) {
  int len = s21_strlen(str);
  char *dot = s21_strchr(str, '.');
  if (dot) {
    for (int i = len - 1; str[i] != '.'; i--) {
      if (str[i] == '0') {
        str[i] = '\0';
      } else {
        break;
      }
    }
    if (dot[1] == '\0') {
      dot[0] = '\0';
    }
  }
}

void s21_g_to_str(char *str, int precision) {
  int normCifry = 0;
  s21_size_t len = s21_strlen(str);
  int flag = 0;
  for (s21_size_t i = 0; i < s21_strlen(str); i++) {
    if ((str[i] == '0' && !flag) || str[i] == '.')
      continue;
    else
      flag = 1;

    if (is_digit(str[i]) && flag) {
      normCifry++;
    }
    if (normCifry == precision && i + 1 < len) {
      int next = str[i + 1] == '.' ? 2 : 1;
      str[i] = str[i + next] - '0' > 5 ? (char)(str[i] + 1) : str[i];
      str[i + 1] = '\0';
      break;
    }
  }
}
void s21_eE_to_str(char *str, long double number, s21_Format *parameters) {
  parameters->sign = number < 0 ? -1 : 1;
  int notation = 0, local_sign = parameters->sign;
  number *= parameters->sign;
  if (number < 1) {
    while (number < 1 && notation-- > -42) {
      number *= 10;
    }
    number < 1 ? (notation = number = 0) : 1;
    if (s21_compare_round(parameters, number)) {
      if (parameters->precision >= 0) {
        --parameters->precision;
      }
      notation++;
    }
    if (s21_strchr("gG", parameters->specifier))
      s21_delete_zero(parameters, str);
    s21_mantisa_to_str(parameters, &str[s21_strlen(str)], notation);
  } else {
    while (number >= 10) {
      notation++;
      number /= 10;
    }
    if (s21_compare_round(parameters, number)) {
      if (parameters->precision >= 0) {
        --parameters->precision;
      }
      notation++;
    }
  }
  number = roundl(number * pow(10, parameters->precision));
  s21_int_to_str(str, number, parameters);
  if (parameters->precision > 0) {
    s21_move_str_right(&str[1]);
    str[1] = '.';
    for (int i = (int)s21_strlen(str); i < 2 + parameters->precision; i++) {
      str[i] = '0';
    }
  }
  if (s21_strchr("gG", parameters->specifier)) s21_delete_zero(parameters, str);
  s21_mantisa_to_str(parameters, &str[s21_strlen(str)], notation);
  parameters->sign = local_sign;
}
void s21_spec_eE(char *str, va_list *dop_values, s21_Format *textformat) {
  long double l_number = 0;
  double number = 0;
  switch (textformat->length) {
    case 'L':
      l_number = va_arg(*dop_values, long double);
      s21_eE_to_str(str, l_number, textformat);
      break;
    default:
      number = va_arg(*dop_values, double);
      s21_eE_to_str(str, number, textformat);
      break;
  }
  s21_work_precision(str, textformat);
  s21_work_flag(str, textformat);
  s21_make_string_width(textformat, str);
}

void s21_mantisa_to_str(s21_Format *parameters, char *mantice, int notation) {
  int start_pos = 0;
  if (parameters->flag_sharp && parameters->precision <= 0) {
    mantice[start_pos++] = '.';
  }
  mantice[start_pos++] = s21_strchr("eg", parameters->specifier) ? 'e' : 'E';
  mantice[start_pos++] = (notation >= 0) ? '+' : '-';
  if (abs(notation) < 10) {
    mantice[start_pos++] = '0';
    s21_int_to_str(&mantice[start_pos], abs(notation), parameters);
  } else {
    s21_int_to_str(&mantice[start_pos], abs(notation), parameters);
  }
}

void s21_delete_zero(s21_Format *parameters, char *str) {
  int pos = (int)s21_strlen(str) - 1;
  if (pos > 0) {
    while (str[pos] == '0') {
      if (!parameters->flag_sharp ||
          (parameters->flag_sharp && pos > parameters->precision)) {
        str[pos] = '\0';
        pos -= 1;
      }
    }
    if (!parameters->flag_sharp && str[pos] == '.') {
      str[pos] = '\0';
    }
  }
}
void s21_spec_f(char *str, va_list *dop_values, s21_Format *textformat) {
  long double l_number = 0;
  double d_number = 0;

  switch (textformat->length) {
    case 'L':
      l_number = va_arg(*dop_values, long double);
      s21_float_to_str(str, textformat, l_number);

      break;

    case 'l':
    default:
      d_number = va_arg(*dop_values, double);
      s21_float_to_str(str, textformat, d_number);
      break;
  }
  s21_work_flag(str, textformat);
  s21_make_string_width(textformat, str);
}

void s21_float_to_str(char *str, s21_Format *textformat, long double number) {
  textformat->sign = number < 0 ? -1 : 1;
  number *= textformat->sign;
  int local_sign = textformat->sign;
  if (textformat->precision <= 0) {
    s21_int_to_str(str, roundl(number), textformat);
  } else {
    long double left = 0, right = 0;
    int pos = 0;
    char result[256] = {0};
    right = modfl(number, &left);
    if (s21_strchr("gG", textformat->specifier) && textformat->precision == 1 &&
        right >= 0.5) {
      left++;
    }
    s21_int_to_str(str, roundl(left), textformat);
    s21_strcat(str, ".");
    int local_precision = textformat->precision;
    if (s21_strchr("gG", textformat->specifier)) {
      if (left >= 1) {
        local_precision = textformat->precision - (int)s21_strlen(str) + 1 +
                          ((int)roundl(number) == 0);
      } else if (right * pow(10, local_precision) < 1) {
        local_precision = 4;
      }
    }
    for (int i = 0; i < local_precision; i++) right *= 10;
    s21_int_to_str(result, roundl(right), textformat);
    pos = (int)s21_strlen(result);
    for (int i = 0; pos < local_precision; i++, pos++) {
      s21_move_str_right(result);
      result[i] = '0';
    }
    if (s21_strchr("gG", textformat->specifier) && (int)roundl(number) != 0) {
      result[textformat->precision - (int)s21_strlen(str) + 1] = '\0';
    }
    s21_strcat(str, result);
  }
  textformat->sign = local_sign;
}

void s21_spec_c(char *str, va_list *dop_values, s21_Format *textformat) {
  if (textformat->length == 'l') {
    wchar_t wchar;
    wchar = (wchar_t)va_arg(*dop_values,
                            int);  // в этом моменте не уверен, надо спросить у
                                   // Ильдара, чар или вчар получаем
    s21_wchar_to_str(str, textformat, wchar);
  } else {
    char ch;
    ch = va_arg(*dop_values, int);
    s21_char_to_str(str, textformat, ch);
  }

  s21_work_flag(str, textformat);
  s21_make_string_width(textformat, str);
}

void s21_char_to_str(char *str, s21_Format *textformat, char ch) {
  if (!textformat->flag_minus && textformat->width) {
    for (int i = 0; i < textformat->width; i++) {
      str[i] = ' ';
      if (i == textformat->width - 1) {
        str[i] = ch;
      }
    }
  } else if (textformat->width) {
    str[0] = ch;
    for (int i = 1; i < textformat->width; i++) {
      str[i] = ' ';
    }
  } else {
    str[0] = ch;
  }
}

void s21_wchar_to_str(char *str, s21_Format *textformat, wchar_t wchar) {
  if (!textformat->flag_minus && textformat->width) {
    char buffer[256] = {'\0'};
    wcstombs(buffer, &wchar, 256);
    for (s21_size_t i = 0; i < textformat->width - s21_strlen(str); i++) {
      str[i] = ' ';
      s21_strcat(str, buffer);
    }
  } else if (textformat->width) {
    wcstombs(str, &wchar, 256);
    for (s21_size_t i = s21_strlen(str); i < (s21_size_t)textformat->width; i++)
      str[i] = ' ';
  } else {
    wcstombs(str, &wchar, 256);
  }
}
int s21_compare_round(s21_Format *parameters, long double number) {
  char compare_str[128] = {'\0'};
  int length = 0, result = 0;
  number *= pow(10, parameters->precision);
  s21_int_to_str(compare_str, number, parameters);
  length = s21_strlen(compare_str);
  number = roundl(number);
  s21_int_to_str(compare_str, number, parameters);
  if (length < (int)s21_strlen(compare_str)) {
    result = 1;
  }
  return result;
}

// D and I

void s21_spec_di(char *str, va_list *dop_values, s21_Format *textformat) {
  long int ld_number = 0;
  short int sd_number = 0;
  int number = 0;

  switch (textformat->length) {
    case 'h':
      sd_number = (short int)va_arg(*dop_values, int);
      s21_int_to_str(str, sd_number, textformat);
      break;
    case 'l':
      ld_number = (long int)va_arg(*dop_values, long int);
      s21_int_to_str(str, ld_number, textformat);
      break;
    default:
      number = va_arg(*dop_values, int);
      s21_int_to_str(str, number, textformat);
      break;
  }
  s21_work_precision(str, textformat);
  s21_work_flag(str, textformat);
  s21_make_string_width(textformat, str);
}
void s21_int_to_str(char *str, long double number, s21_Format *parameters) {
  char *point = s21_NULL;
  int flag = 1;
  parameters->sign = number < 0 ? -1 : 1;
  number *= parameters->sign;
  point = str;
  if (parameters->specifier != 'n') {
    while (flag) {
      int digit = 0;
      if (number >= 10) {
        digit = (int)fmod(number, 10);
        number = (number - digit) / 10;
      } else {
        digit = (int)number;
        flag = 0;
      }
      *(point++) = (char)(digit + '0');
    }
    if (s21_strchr("dioxX", parameters->specifier) &&
        parameters->precision == 0 && number == 0 && parameters->point)
      str[0] = '\0';
    *point = '\0';
    s21_reverse_writing(str);
  }
}
/* Функция обработки ширины форматной строки */
void s21_make_string_width(s21_Format *parameters, char *str) {
  int pos_str = (int)s21_strlen(str);
  if (parameters->width > pos_str) {
    str[parameters->width + 1] = '\0';
    while (pos_str >= 0) {
      str[parameters->width--] = str[pos_str--];
    }
    for (pos_str = 0; pos_str <= parameters->width; pos_str++) {
      if (s21_strchr("diopuxX", parameters->specifier)) {
        str[pos_str] =
            (parameters->flag_zero && !parameters->point) ? '0' : ' ';
      }
      if (s21_strchr("feEgGcs", parameters->specifier)) {
        if (parameters->flag_zero) {
          str[pos_str] = '0';
        } else {
          str[pos_str] = ' ';
        }
      }
    }
  }
}
/*проверка на флаги*/
bool is_flag(char ch) {
  bool res = false;
  if (((ch) == '-') || ((ch) == '+') || ((ch) == ' ') || ((ch) == '#') ||
      ((ch) == '0')) {
    res = true;
  }
  return res;
}
/* проверка на ширину*/
bool is_digit(char ch) {
  bool res = false;
  if ((ch) >= '0' && (ch) <= '9') {
    res = true;
  }
  return res;
}
/* проверка на длину */
bool is_length(char ch) {
  bool res = false;
  if ((ch) == 'h' || (ch) == 'l' || (ch) == 'L') {
    res = true;
  }
  return res;
}
/* проверка на спецификатор*/
bool is_specifier(char ch) {
  bool res = false;
  if ((ch) == 'c' || (ch) == 'd' || (ch) == 'i' || (ch) == 'e' || (ch) == 'E' ||
      (ch) == 'f' || (ch) == 'g' || (ch) == 'G' || (ch) == 'o' || (ch) == 's' ||
      (ch) == 'u' || (ch) == 'x' || (ch) == 'X' || (ch) == 'p' || (ch) == 'n' ||
      (ch) == '%') {
    res = true;
  }
  return res;
}
