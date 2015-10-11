#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "lexer.h"


int main() {
  TOKEN* t;

  while(!feof(stdin)) {
    for(t = get_tokens(stdin); t != NULL; t = t->right) {
      __info("|%s| ", t->token);
      free(t->token);
      free(t);
    }
    __info("\n");
  }

  return 0;
}
