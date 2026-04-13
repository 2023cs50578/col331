#include "types.h"
#include "stat.h"
#include "user.h"

char* strcpy(char *s, const char *t) {
  char *os = s;
  while((*s++ = *t++) != 0);
  return os;
}

char* strchr(const char *s, char c) {
  for(; *s; s++) if(*s == c) return (char*)s;
  return 0;
}

void* memset(void *dst, int c, uint n) {
  char *cdst = (char *) dst;
  uint i;
  for(i = 0; i < n; i++) cdst[i] = c;
  return dst;
}

void* memmove(void *dst, const void *src, uint n) {
  const char *s = src;
  char *d = dst;
  if(s < d && s + n > d){
    s += n; d += n;
    while(n-- > 0) *--d = *--s;
  } else {
    while(n-- > 0) *d++ = *s++;
  }
  return dst;
}

uint strlen(const char *s) {
  uint n;
  for(n = 0; s[n]; n++);
  return n;
}
