
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>


int main() {

    token* t = token_create_string("test token", 0, 1);
    token* b = token_create_int(1, 1, 2);
    token* a = token_create_float(1.3 , 4, 5);
    token* c = token_create_name("test name", 4, 5);
    token* d = token_create_operator("unary", 6, 6);

    token_print(t);
    token_print(b);
    token_print(a);
    token_print(c);
    token_print(d);

    token_free(a);
    token_free(b);
    token_free(c);
    token_free(d);
    token_free(t);
    
    printf("\n");
    token_list* toks = tokenize("ADADA EEE     AA");
    print_token_list(toks);

    return 1;

}

