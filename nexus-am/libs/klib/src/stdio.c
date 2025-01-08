#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  //return 0;
  va_list ap;
  va_start(ap, fmt);
//  int cnt = vsprintf(fmt, ap);
  char buf[1024] = {0};
  int cnt = vsprintf(buf, fmt, ap);
  for(int i = 0; i < cnt; i++) {
    _putc(buf[i]);
  }
  va_end(ap);
  return cnt;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
//  return 0;
  int cnt = 0;
  //% d s c x u 
  while(*fmt) {
    if(*fmt != '%') {
      *out = *fmt;
      out++;
      fmt++;
      cnt++;
    } else {
      fmt++;
      switch(*fmt) {
        case 'd': {
          int num = va_arg(ap, int);
          char buf[32] = {0};
          int i = 0;
          if(num < 0) {
            *out = '-';
            out++;
            num = -num;
          }
          if(num == 0) {
            *out = '0';
            out++;
            cnt++;
          }
          while(num) {
            buf[i] = num % 10 + '0';
            num /= 10;
            i++;
          }
          for(int j = i - 1; j >= 0; j--) {
            *out = buf[j];
            out++;
            cnt++;
          }
          break;
        }
        case 's': {
          char *str = va_arg(ap, char *);
          while(*str) {
            *out = *str;
            out++;
            str++;
            cnt++;
          }
          break;
        }
        case 'c': {
          char c = va_arg(ap, int);
          *out = c;
          out++;
          cnt++;
          break;
        }
        case 'x': {
          unsigned int num = va_arg(ap, unsigned int);
          char buf[32] = {0};
          int i = 0;
          if(num == 0) {
            *out = '0';
            out++;
            cnt++;
          }
          while(num) {
            if(num % 16 < 10) {
              buf[i] = num % 16 + '0';
            } else {
              buf[i] = num % 16 - 10 + 'a';
            }
            num /= 16;
            i++;
          }
          for(int j = i - 1; j >= 0; j--) {
            *out = buf[j];
            out++;
            cnt++;
          }
          break;
        }
        case 'u': {
          unsigned int num = va_arg(ap, unsigned int);
          char buf[32] = {0};
          int i = 0;
          if(num == 0) {
            *out = '0';
            out++;
            cnt++;
          }
          while(num) {
            buf[i] = num % 10;
            num /= 10;
            i++;
            }
          for(int j = i - 1; j >= 0; j--) {
            *out = buf[j];
            out++;
            cnt++;
          }
          break;
        }
        default: break;
      }
      fmt++;
    }
  }
  *out = '\0';
  return cnt;
}


int sprintf(char *out, const char *fmt, ...) {
  //return 0;
  va_list ap;
  va_start(ap, fmt);
  int cnt = vsprintf(out, fmt, ap);
  va_end(ap);
  return cnt;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  //return 0;
  va_list ap;
  va_start(ap, fmt);
  int cnt = vsprintf(out, fmt, ap);
  va_end(ap);
  if(cnt > n)
  {
    out[n] = '\0';
    return n;
  }
  return cnt;
}

#endif
