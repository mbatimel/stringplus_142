#ifndef SRC_S21_STRING_H_
#define SRC_S21_STRING_H_
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
/*----------------------------------------------------------------*/
typedef long unsigned s21_size_t;
/*----------------------------------------------------------------*/
#define s21_NULL ((void *)0)

/*----------------------------------------------------------------*/
s21_size_t s21_strlen(const char *str);
void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int c);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strtok(char *str, const char *delim);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strpbrk(const char *str1, const char *str2);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
char *s21_strrchr(const char *str, int c);
char *s21_strerror(int errnum);
/*----------------------------------------------------------------*/
// func from C#
void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);
char *s21_strcpy(char *dest, const char *src);
/*----------------------------SPRINTF------------------------------*/
// main func
typedef struct s21_Format {
  bool flag_minus;  // Выравнивание по левому краю
  bool flag_plus;  // Заставляет предварять результат знаком + или -
  bool flag_space;  // Если знак не записан, перед значением
                    // вставляется пробел.
  bool flag_sharp;  // При o, x или X значению предшествуют 0, 0x или
                    //  0X соответственно, если не нуль. При с e, E и
                    //  f заставляет записанный вывод содержать
                    // десятичную точку. По умолчанию, если цифры не
                    // следуют, десятичная точка не записывается.
  bool flag_zero;  // Дополняет число слева нулями (0) вместо
                   // пробелов
  int width;  // Минимальное количество печатаемых цифр.
  int precision;  // Минимальное количество записываемых цифр.
  int precision_set;
  char length;     // Тип переменной
  char specifier;  // Спецификатор

  int sign;    // Знак + или -
  bool point;  // ФЛАГ НА ТООООЧКУУ!

} s21_Format;

int s21_sprintf(char *str, const char *format, ...);
char *treatment(char *str, s21_Format *textformat, va_list *dop_valies,
                s21_size_t *len);
char *s21_strcat(char *dest, const char *src);
void s21_o_spec(char *str_res, va_list *dop_valies, s21_Format *textformat);
void s21_s_spec(char *str, va_list *dop_value, s21_Format *textformat);
void s21_u_spec(char *str_res, va_list *dop_value, s21_Format *textformat);
void s21_xX_spec(char *str_res, va_list *dop_value, s21_Format *textformat);
void s21_p_spec(char *str_res, va_list *dop_value, s21_Format *textformat);
void s21_n_spec(s21_size_t const *str_len, va_list *dop_value);
void s21_prosent_spec(char *str, s21_Format *textformat);
void s21_spec_di(char *str, va_list *dop_values, s21_Format *textformat);
void s21_spec_c(char *str, va_list *dop_values, s21_Format *textformat);
void s21_spec_f(char *str, va_list *dop_values, s21_Format *textformat);
void s21_spec_eE(char *str, va_list *dop_values, s21_Format *textformat);
void s21_spec_gG(char *str, va_list *dop_values, s21_Format *textformat);

void s21_g_to_str(char *str, int precision);
void treatment_for_zeroes(char *str);
void s21_make_string_width(s21_Format *parameters, char *str);
void s21_int_to_str(char *str, long double number, s21_Format *parameters);
void s21_wchar_to_str(char *str, s21_Format *textformat, wchar_t wchar);
void s21_char_to_str(char *str, s21_Format *textformat, char ch);
void s21_float_to_str(char *str, s21_Format *textformat, long double number);
void s21_mantisa_to_str(s21_Format *parameters, char *mantice, int notation);
void s21_delete_zero(s21_Format *parameters, char *str);
int s21_compare_round(s21_Format *parameters, long double number);
void s21_gG_to_str(char *str, long double number, s21_Format *parameters);
void s21_eE_to_str(char *str, long double number, s21_Format *parameters);
void s21_ol_to_str(char *str_res, unsigned long long number,
                   s21_Format *textformat);
void s21_reverse_writing(char *str);
void s21_ul_to_str(char *str_res, unsigned long long number,
                   s21_Format *textformat);
void s21_xl_to_str(char *str, unsigned long long number,
                   s21_Format *parameters);
void s21_check_flags(const char *str_res, s21_Format *textformat);
void s21_work_flag(char *str_res, s21_Format *textformat);
void s21_move_str_right(char *str_res);
void s21_sharp_work(char *str_res, s21_Format *textformat);
char *s21_str_to_int(int *number, const char *format, va_list *input);
void s21_default_precision(s21_Format *parameters);
void s21_work_precision(char *str_res, s21_Format *textformat);

bool is_specifier(char ch);
bool is_length(char ch);
bool is_digit(char ch);
bool is_flag(char ch);
bool is_digit_o2(char ch);
bool is_digit_o(char ch);

/*----------------------------SSCANF------------------------------*/
#ifndef ULONG_MAX
#define ULONG_MAX ((unsigned long)(~0L)) /* 0xFFFFFFFF */
#endif

#ifndef LONG_MAX
#define LONG_MAX ((long)(ULONG_MAX >> 1)) /* 0x7FFFFFFF */
#endif

#ifndef LONG_MIN
#define LONG_MIN ((long)(~LONG_MAX)) /* 0x80000000 */
#endif

struct step {
  int strStep;
  int strLength;
  int formatStep;
  int countReturn;
};

struct arguments {
  short star;
  int width;
  char modifier;
  int flag;
  int flag_o;
};

int s21_sscanf(const char *str, const char *format, ...);
int s21_switchCase(va_list ap, struct arguments *Arguments, struct step *Step,
                   const char *str, const char *format);
int s21_specifier_c(va_list ap, const char *str, struct step *Step,
                    struct arguments *args);
int s21_specifier_s(va_list ap, const char *str, struct step *Step,
                    struct arguments *args);
int s21_specifier_i(va_list ap, const char *str, struct step *Step,
                    struct arguments *args);
int s21_specifier_f_e_E_g_G(va_list ap, const char *str, struct step *Step,
                            struct arguments *args);
int s21_specifier_percent(const char *str, struct step *Step,
                          struct arguments *args);
int s21_specifier_n(va_list ap, const char *str, struct step *Step,
                    struct arguments *args, const char *format);
int s21_Euler_search(const char *str, char **end, long double *num);
int s21_string_to_Ld(const char *str, char **end, long double *numbur,
                     struct arguments *args);
int s21_inf_nan(const char *str, int i, long double *num);
long long s21_strtol(const char *nptr, char **endptr, register int base);
int s21_is_space(char c);
int s21_is_digit(char c);

int s21_sscanf_atoi_d(const char *str, struct arguments *args, long *res,
                      struct step *Step);
int s21_specifier_d(va_list valist, const char *str, struct arguments *args,
                    struct step *Step);

double s21_pow(double x, int power);
int s21_specifier_u(va_list valist, const char *str, struct arguments *args,
                    struct step *Step);
int s21_specifier_o(va_list valist, const char *str, struct arguments *args,
                    struct step *Step);
void s21_specifier_result(va_list valist, struct arguments *args,
                          long int *result);
int s21_specifier_Xx(va_list valist, const char *str, struct arguments *args,
                     struct step *Step, int *end_func);
int s21_specifier_p(va_list valist, const char *str, struct arguments *args,
                    struct step *Step, int *end_func);
int s21_checkSymbols_XxP(const char *str, int *symbols_counter, long *decimal,
                         struct arguments *args, int *end_func, int *flagx);
#endif
