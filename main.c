#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "lexer.h"

int main()
{
  TOKEN* t;

  while(!feof(stdin))
  {
    for(t = tokenize(stdin); t != NULL; t = t->right) {
      printf("\"%s\":%x ", t->token, t->type);
      token_destroy(t);
    }
    printf("\n");
  }

  return 0;
}
