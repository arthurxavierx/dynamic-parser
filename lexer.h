#ifndef __LEXER_H__
#define __LEXER_H__

#include <stdio.h>
#include <stdlib.h>

/**
 * TOKEN_TYPE
 */
#define TOKEN_TYPE unsigned int

#define TOKEN_NULL    0x000000u  // 0000 0000 0000 0000 0000 0000
#define TOKEN_ANY     0xFFFFFFu  // 1111 1111 1111 1111 1111 1111
#define TOKEN_NEWLINE 0xFFFFFEu  // 1111 1111 1111 1111 1111 1110

// expression
#define TOKEN_EXPRESSION    0x000100u  // 0000 0000 0000 0001 0000 0000
#define TOKEN_OPERATOR_ADD  0x000101u  // 0000 0000 0000 0001 0000 0001
#define TOKEN_OPERATOR_SUB  0x000102u  // 0000 0000 0000 0001 0000 0010

// term
#define TOKEN_TERM         0x001000u  // 0000 0000 0001 0000 0000 0000
#define TOKEN_NUMBER       0x001001u  // 0000 0000 0001 0000 0000 0001
#define TOKEN_PARENTHESIS  0x001002u  // 0000 0000 0001 0000 0000 0010

#define TOKEN_BEGIN        0x000010u  // 0000 0000 0000 0000 0001 0000
#define TOKEN_END          0x000010u  // 0000 0000 0000 0000 0010 0000

/**
 * TOKEN struct
 */
typedef struct TOKEN {
  void* value;
  char* token;
  TOKEN_TYPE type;
  struct TOKEN *left, *right;
} TOKEN;

typedef struct LEXICAL_RULE {
  const char* string;
  int (*function)(const char*);
  TOKEN_TYPE type;
} LEXICAL_RULE;

static const LEXICAL_RULE DICTIONARY[] = {
  { "\n", NULL, TOKEN_NEWLINE },
  { NULL, isnumber_, TOKEN_NUMBER },
  { "+", NULL, TOKEN_OPERATOR_ADD },
  { "-", NULL, TOKEN_OPERATOR_SUB },
  { "(", NULL, TOKEN_PARENTHESIS | TOKEN_BEGIN },
  { ")", NULL, TOKEN_PARENTHESIS | TOKEN_END },
  { NULL, NULL, TOKEN_NULL }
};


int istype(TOKEN_TYPE t1, TOKEN_TYPE t2);
int isnumber_(const char* s);

TOKEN* make_token(char* s);
TOKEN* get_tokens(FILE* in);

#endif
