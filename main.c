
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>


int main() {
    char* input = "int main(){ int a= 0; a++; return(0); }";
    
    printf("tokenizing %s \n", input);

    token_list* toks = tokenize(input);
    print_token_list(toks);
    token_list_free(toks);
    return 1;
}

