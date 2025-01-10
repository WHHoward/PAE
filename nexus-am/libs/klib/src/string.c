#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  //return 0;
  int cnt = 0;
  while(*s) {
    cnt++;
    s++;
  }
  return cnt;
}

char *strcpy(char* dst,const char* src) {
  //return NULL;
  char *ret = dst;
  while(*src) {
    *dst = *src;
    dst++;
    src++;
  }
  *dst = '\0';
  return ret;
}

char* strncpy(char* dst, const char* src, size_t n) {
//  return NULL;
  char *ret = dst;
  while(*src && n) {
    *dst = *src;
    dst++;
    src++;
    n--;
  }
  *dst = '\0';
  return ret;
}

char* strcat(char* dst, const char* src) {
  //return NULL;
  char *ret = dst;
  while(*dst) {
    dst++;
  }
  while(*src) {
    *dst = *src;
    dst++;
    src++;
  }
  *dst = '\0';
  return ret;
}

int strcmp(const char* s1, const char* s2) {
  //return 0;
  while(*s1 && *s2) {
    if(*s1 != *s2) {
      return *s1 - *s2;
    }
    s1++;
    s2++;
  }
  return *s1 - *s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  //return 0;
  if(n <= 0) {
    return 0;
  }
  while(*s1 && *s2 && n) {
    if(*s1 != *s2) {
      return *s1 - *s2;
    }
    s1++;
    s2++;
    n--;
  }
  return *s1 - *s2;
}

void* memset(void* v,int c,size_t n) {
  //return NULL;
  char *p = (char *)v;
  while(n) {
    *p = c;
    p++;
    n--;
  }
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  //return NULL;
  char *p = (char *)out;
  char *q = (char *)in;
  while(n) {
    *p = *q;
    p++;
    q++;
    n--;
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  //return 0;
  char *p = (char *)s1;
  char *q = (char *)s2;
  while(n) {
    if(*p != *q) {
      return *p - *q;
    }
    p++;
    q++;
    n--;
  }
  return 0;
}

#endif
