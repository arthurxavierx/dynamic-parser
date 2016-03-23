#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "utils.h"
#include "lexer.h"

static const TOKEN TOKEN_EMPTY = {NULL, NULL, TOKEN_NULL, NULL, NULL};

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

int lexical_rule_compare(const LEXICAL_RULE* rule, const char* t)
{
  return rule->function != NULL && rule->function(t, rule->string) > 0;
}

static unsigned char c = 0;

TOKEN get_token(const LEXICAL_RULE* dictionary, FILE* in)
{
  unsigned char r = 0; /**< index of current dictionary rule */

  char *t; /**< pointer to the start of the whole sequence read from in */
  char *s; /**< pointer to the current character in the token string */

  TOKEN token = TOKEN_EMPTY; /**< current token */

  // initialize token string. { max length of token = 255 }
  t = (char*)malloc(255);
  memset(t, 0, 255);

  s = t;

  // reads a sequence of characters
  do {
    // ignores whitespace that's not \n
    if(c == 0 || (c != '\n' && isspace(c)))
      continue;

    // sets the current character in the token string
    *s = c;

    // (type != NULL)? ==> check if the character read has the same token type
    if(token.type != TOKEN_NULL) {
      // (token type of t != type)? ==> instantiate read token and starting reading a new one
      if(!lexical_rule_compare(&dictionary[r], t)) {
        // terminates current token string
        *s = 0;
        __info("'%s' : %x\n", t, token.type);

        token.token = (char*)malloc(s - t);
        memcpy(token.token, t, s - t);
        return token;
      }
    }

    // (type == NULL)? ==> search for a matching lexical rule in dictionary
    if(token.type == TOKEN_NULL) {
      for(r = 0; dictionary[r].type != TOKEN_NULL; r++) {
        if(lexical_rule_compare(&dictionary[r], t)) {
          token.type = dictionary[r].type;
          break;
        }
      }
    }

    // character read into the token string ==> next!
    s++;

  } while(!feof(in) && (c = fgetc(in)) != 0);

  if(token.type == TOKEN_NULL && !feof(in)) {
    __error(RED "ERROR:" RESET " Unknown token \"%s\"\n", t);
    return TOKEN_EMPTY;
  }
  __info("'%s' : %x\n", t, token.type);

  free(t);

  return token;
}
