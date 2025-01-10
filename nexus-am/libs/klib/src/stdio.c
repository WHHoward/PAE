#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  char buf[1024] = {0};
  int cnt = vsprintf(buf, fmt, ap);
  for(int i = 0; i < cnt; i++) {
    _putc(buf[i]);
  }
  va_end(ap);
  return cnt;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int cnt = 0;
  for(int i = 0; fmt[i]; i++) {
    if(fmt[i] != '%') {
      out[cnt++] = fmt[i];
    } else {
      i++;
      switch(fmt[i]) {
        case 'd': {
          int num = va_arg(ap, int);
          if(num < 0) {
            out[cnt++] = '-';
            num = -num;
          }
          char tmp[32] = {0};
          int len = 0;
          if(num == 0) {
            tmp[len++] = '0';
          }
          while(num) {
            tmp[len++] = num % 10 + '0';
            num /= 10;
          }
          for(int j = len - 1; j >= 0; j--) {
            out[cnt++] = tmp[j];
          }
          break;
        }
        case 's': {
          const char *str = va_arg(ap, const char *);
          for(int j = 0; str[j]; j++) {
            out[cnt++] = str[j];
          }
          break;
        }
        case 'c': {
          char ch = va_arg(ap, int);
          out[cnt++] = ch;
          break;
        }
        case 'x': {
          unsigned int num = va_arg(ap, unsigned int);
          if(num < 0){
            out[cnt++] = '-';
            num = -num;
          }
          char tmp[32] = {0};
          int len = 0;
          if(num == 0) {
            tmp[len++] = '0';
          }
          while(num) {
            if(num % 16 < 10) {
              tmp[len++] = num % 16 + '0';
            } else {
              tmp[len++] = num % 16 - 10 + 'a';
            }
            num /= 16;
          }
          out[cnt++] = '0';
          out[cnt++] = 'x';
          for(int j = len - 1; j >= 0; j--) {
            out[cnt++] = tmp[j];
          }
          break;
        }
        case 'u': {
          unsigned int num = va_arg(ap, unsigned int);
          if(num < 0){
            out[cnt++] = '-';
            num = -num;
          }
          char tmp[32] = {0};
          int len = 0;
          if(num == 0) {
            tmp[len++] = '0';
          }
          while(num) {
            tmp[len++] = num % 10 + '0';
            num /= 10;
          }
          for(int j = len - 1; j >= 0; j--) {
            out[cnt++] = tmp[j];
          }
          break;
        }
        default: break;
      }
    }
  }
  out[cnt] = '\0';
  return cnt;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int cnt = vsprintf(out, fmt, ap);
  va_end(ap);
  return cnt;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int cnt = vsprintf(out, fmt, ap);
  va_end(ap);
  if(cnt > n) {
    out[n] = '\0';
    return n;
  }
  return cnt;
}

#endif
