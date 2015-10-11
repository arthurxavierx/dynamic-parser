#define NDEBUG
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "utils.h"
#include "lexer.h"

int istype(unsigned int t1, unsigned int t2)
{
  return !((t1 & t2) == t1);
}

TOKEN* make_token(char* s)
{
  TOKEN* t = (TOKEN*)malloc(sizeof(TOKEN));
  t->token = s;
  t->left = NULL;
  t->right = NULL;
  t->value = NULL;
  t->type = TOKEN_NULL;
  return t;
}


TOKEN* get_tokens(FILE* in)
{
  unsigned char i;
  /** current character */
  char c;
  /** pointer to the start of the whole sequence read from in */
  char *t;
  /** pointer to the current character in the token string */
  char *s;
  /** current token type */
  TOKEN_TYPE type;

  TOKEN *first_token = NULL, *token = NULL, *last_token = NULL;

  // initialize token string
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

    // (type != NULL) ==> check if the character read has the same token type
    if(type != TOKEN_NULL) {

      // (token type of c != type) ==> instantiate read token and starting reading a new one
      if(!(DICTIONARY[i].string != NULL && c == DICTIONARY[i].string[s - t])
         && !(DICTIONARY[i].function != NULL && DICTIONARY[i].function(t) != 0)) {

        // terminates current token string
        *s = 0;
        __info("'%s' : %x\n", t, type);

        last_token = token;
        token = make_token(NULL);
        token->token = (char*)malloc(s - t);
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
      for(i = 0; DICTIONARY[i].type != TOKEN_NULL; i++) {

        __info("-> [%ld]%c == [%d]%c\n", s - t, c, i, DICTIONARY[i].string != NULL ? DICTIONARY[i].string[s - t] : 'f');

        // (c in dic.string or dic.function(c) == true) ==> type = dic.type
        if((DICTIONARY[i].string != NULL && c == DICTIONARY[i].string[s - t])
           || (DICTIONARY[i].function != NULL && DICTIONARY[i].function(t) != 0)) {

          __info("found %c == %c : %x\n", c, DICTIONARY[i].string != NULL ? DICTIONARY[i].string[s - t] : 'f', DICTIONARY[i].type);

          type = DICTIONARY[i].type;
          break;
        }
      }
    }

    // ends reading when eof or \n
    if(feof(in) || c == '\n')
      break;

    // character read into the token string ==> next!
    s++;
  }
  __info("'%s' : %x\n", t, type);

  free(t);

  return first_token;
}
