#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "lexer.h"

int strequal(const char* a, const char* b);
int isnumberstr(const char* s, const char*);

// expression
#define TOKEN_EXPRESSION    0x000100u  // 0000 0000 0000 0001 0000 0000
#define TOKEN_OPERATOR_ADD  0x000101u  // 0000 0000 0000 0001 0000 0001
#define TOKEN_OPERATOR_SUB  0x000102u  // 0000 0000 0000 0001 0000 0010

// term
#define TOKEN_TERM         0x001000u  // 0000 0000 0001 0000 0000 0000
#define TOKEN_NUMBER       0x001001u  // 0000 0000 0001 0000 0000 0001
#define TOKEN_PARENTHESIS  0x001002u  // 0000 0000 0001 0000 0000 0010

#define TOKEN_BEGIN        0x000010u  // 0000 0000 0000 0000 0001 0000
#define TOKEN_END          0x000020u  // 0000 0000 0000 0000 0010 0000

const LEXICAL_RULE DICTIONARY[] = {
  { "\n", strequal,    TOKEN_NEWLINE },
  { NULL, isnumberstr, TOKEN_NUMBER },
  { "+",  strequal,    TOKEN_OPERATOR_ADD },
  { "-",  strequal,    TOKEN_OPERATOR_SUB },
  { "(",  strequal,    TOKEN_PARENTHESIS | TOKEN_BEGIN },
  { ")",  strequal,    TOKEN_PARENTHESIS | TOKEN_END },
  { NULL, NULL, TOKEN_NULL }
};

/**
 * Tokenize input stream
 */
int main()
{
  TOKEN* t;

  while(!feof(stdin))
  {
    for(t = tokenize(DICTIONARY, stdin); t != NULL; t = t->right) {
      printf("\"%s\":%x ", t->token, t->type);
      token_destroy(t);
    }
    printf("\n");
  }

  return 0;
}

/**
 * Check if both strings are equal.
 * @return true if both strings are equal, otherwise false
 */
int strequal(const char* a, const char* b) {
  return strcmp(a, b) == 0;
}

/**
 * Check if a string is a number-like string.
 * @param s string to check
 * @return true if s is a number-like string, otherwise false
 */
int isnumberstr(const char* s, const char* _)
{
  if(s == NULL || *s == '\0' || isspace(*s))
    return 0;
  char* p;
  strtod(s, &p);
  return *p == '\0';
}
