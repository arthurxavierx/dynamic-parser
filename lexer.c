#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "utils.h"
#include "lexer.h"

/**
 * Find a specific lexical rule in the supplied dictionary which matches the specified token.
 * @param dictionary list of lexical rules
 * @param t pointer to the start of the token string
 * @param s pointer to the end (current character) of the token string
 * @return Index of the matched lexical rule
 */
unsigned char match_rule(const LEXICAL_RULE* dictionary, const char*, const char*);

int istype(unsigned int t1, unsigned int t2)
{
  return !((t1 & t2) == t1);
}

TOKEN* token_create(char* s)
{
  TOKEN* t = (TOKEN*)malloc(sizeof(TOKEN));
  t->token = s;
  t->left = NULL;
  t->right = NULL;
  t->value = NULL;
  t->type = TOKEN_NULL;
  return t;
}

void token_destroy(TOKEN* t)
{
  t->left != NULL ? t->left->right = NULL : NULL;
  t->right != NULL ? t->right->left = NULL : NULL;
  free(t->token);
  free(t);
}

TOKEN* tokenize(const LEXICAL_RULE* dictionary, FILE* in)
{
  unsigned char r = 0; /**< index of current dictionary rule */

  unsigned char c;  /**< current character */
  char *t; /**< pointer to the start of the whole sequence read from in */
  char *s; /**< pointer to the current character in the token string */

  TOKEN_TYPE type; /**< current token type */
  TOKEN *token = NULL, *first_token = NULL, *last_token = NULL;

  // initialize token string. { max length of token = 255 }
  t = (char*)malloc(255);
  memset(t, 0, 255);

  s = t;
  type = TOKEN_NULL;

  // reads a sequence of characters
  while((c = fgetc(in)) != 0) {
    // ignores whitespace that's not \n
    if(c != '\n' && isspace(c))
      continue;

    // sets the current character in the token string
    *s = c;

    // (type != NULL)? ==> check if the character read has the same token type
    if(type != TOKEN_NULL) {
      // (token type of c != type)? ==> instantiate read token and starting reading a new one
      if(!(dictionary[r].string != NULL && c == dictionary[r].string[s - t])
         && !(dictionary[r].function != NULL && dictionary[r].function(t) != 0))
      {
        // terminates current token string
        *s = 0;
        __info("'%s' : %x\n", t, type);

        last_token = token;
        token = token_create(NULL);
        token->token = (char*)malloc(s - t);
        token->type = type;
        memcpy(token->token, t, s - t);
        token->left = last_token;
        if(last_token != NULL) last_token->right = token;
        if(first_token == NULL) first_token = token;

        // start again from the beginning
        memset(t, 0, s - t);
        s = t;
        *s = c;
        type = TOKEN_NULL;
      }
    }

    // (type == NULL) ==> search for a matching lexical rule in dictionary
    if(type == TOKEN_NULL) {
      r = match_rule(dictionary, t, s);
      type = dictionary[r].type != TOKEN_NULL ? dictionary[r].type : type;
    }

    // ends reading when eof or \n
    if(feof(in) || c == '\n')
      break;

    // character read into the token string ==> next!
    s++;
  }

  if(type == TOKEN_NULL && !feof(in)) {
    __error(RED "ERROR:" RESET " Unknown token \"%s\"\n", t);
    return NULL;
  }
  __info("'%s' : %x\n", t, type);

  free(t);

  return first_token;
}

unsigned char match_rule(const LEXICAL_RULE* dictionary, const char* t, const char* s)
{
  unsigned char r;
  const unsigned long length = s - t;

  for(r = 0; dictionary[r].type != TOKEN_NULL; r++) {
    // (token length >= dictionary string length) ==> next rule
    if(dictionary[r].string != NULL && length >= strlen(dictionary[r].string))
      continue;

    __info("match [%ld]'%c' against [%d]'%c'\n", length, *s, r, dictionary[r].string != NULL ? dictionary[r].string[length] : 'f');

    // (t in dict.string or dict.function(t) == true) ==> type = dict.type
    if((dictionary[r].string != NULL && strcmp(t, dictionary[r].string) == 0)
       || (dictionary[r].function != NULL && dictionary[r].function(t) != 0))
    {
      __info("found %c == %c : %x\n", *s, dictionary[r].string != NULL ? dictionary[r].string[length] : 'f', dictionary[r].type);
      break;
    }
  }

  return r;
}
