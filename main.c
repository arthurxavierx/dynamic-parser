#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "lexer.h"

int _strcmp(const char* a, const char* b);
int _isnumber(const char* s, const char*);

// expression
#define TOKEN_EXPRESSION    0x000100u  // 0000 0000 0000 0001 0000 0000
#define TOKEN_OPERATOR_ADD  0x000101u  // 0000 0000 0000 0001 0000 0001
#define TOKEN_OPERATOR_SUB  0x000102u  // 0000 0000 0000 0001 0000 0010
#define TOKEN_FUNCTION      0x000104u

// term
#define TOKEN_TERM         0x001000u  // 0000 0000 0001 0000 0000 0000
#define TOKEN_NUMBER       0x001001u  // 0000 0000 0001 0000 0000 0001
#define TOKEN_PARENTHESIS  0x001002u  // 0000 0000 0001 0000 0000 0010

#define TOKEN_BEGIN        0x000010u  // 0000 0000 0000 0000 0001 0000
#define TOKEN_END          0x000020u  // 0000 0000 0000 0000 0010 0000

const LEXICAL_RULE DICTIONARY[] = {
  { "\n", _strcmp,    TOKEN_NEWLINE },
  { NULL, _isnumber,  TOKEN_NUMBER },
  { "+",  _strcmp,    TOKEN_OPERATOR_ADD },
  { "-",  _strcmp,    TOKEN_OPERATOR_SUB },
  { "(",  _strcmp,    TOKEN_PARENTHESIS | TOKEN_BEGIN },
  { ")",  _strcmp,    TOKEN_PARENTHESIS | TOKEN_END },
  { NULL, NULL, TOKEN_NULL }
};

/**
 * Tokenize input stream
 */
int main()
{
  TOKEN token;

  while(!feof(stdin)) {
    token = get_token(DICTIONARY, stdin);
    printf("\"%s\":%x\n", token.token, token.type);
  }

  return 0;
}

/**
 * Check if both strings are equal.
 * @return true if both strings are equal, otherwise false
 */
int _strcmp(const char* a, const char* b)
{
  return strcmp(a, b) == 0;
}

/**
 * Check if a string is a number-like string.
 * @param s string to check
 * @return true if s is a number-like string, otherwise false
 */
int _isnumber(const char* s, const char* _)
{
  if(s == NULL || *s == '\0' || isspace(*s))
    return 0;
  char* p;
  strtod(s, &p);
  return *p == '\0';
}
