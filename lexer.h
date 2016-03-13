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

/**
 * Check if a token type partially matches another token type.
 * @param t1 token type to be matched
 * @param t2 token type to match against
 * @return true if t1 is the same type of t2 or if it's a subtype of t2
 */
int istype(TOKEN_TYPE t1, TOKEN_TYPE t2);

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
  int (*function)(const char*, const char*);
  TOKEN_TYPE type;
} LEXICAL_RULE;


/**
 * Create a default TOKEN structure out of a token string.
 * @param s pointer to the token string
 * @return The created TOKEN structure
 */
TOKEN* token_create(char* s);
/**
 * Destroy a TOKEN structure and remove its references from its neighbors.
 * @param t pointer to the token
 */
void token_destroy(TOKEN* t);

/**
 * Generate a token linked list out of an input stream of characters.
 * @param in input stream of characters
 * @return The first token of the generated token list
 */
TOKEN* tokenize(const LEXICAL_RULE* dictionary, FILE* in);

#endif
