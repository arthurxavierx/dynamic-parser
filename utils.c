#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

int isnumber_(const char* s) {
  if(s == NULL || *s == '\0' || isspace(*s))
    return 0;
  char* p;
  strtod(s, &p);
  return *p == '\0';
}

char* ltrim(char* s) {
   while(isspace(*s)) s++;
   return s;
}
char* rtrim(char* s) {
   char* back = s + strlen(s);
   while(isspace(*--back));
   *(back+1) = '\0';
   return s;
}
char* trim(char* s) {
   return rtrim(ltrim(s));
}
