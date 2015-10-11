#ifndef __UTILS_H__
#define __UTILS_H__

#ifndef NDEBUG
  #define __info(...) fprintf(stderr, __VA_ARGS__)
#else
  #define __info(...) ((void)0);
#endif

int isnumber_(const char*);

char* ltrim(char*);
char* rtrim(char*);
char* trim(char*);

#endif
