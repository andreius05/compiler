#include <stdio.h>
#include "lexer.h"


int main(){
    FILE * file = fopen("test.unn","r");
    TokenArray_t * tok_arr = lexer(file);
    for (int i = 0; i < tok_arr->count; i++) {
     Token_t *tok = tok_arr->tokens[i];
     print_token(tok);
    }
    free_token_array(tok_arr);
    fclose(file);
}
