#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

token* token_ctr(enum token_type type, int from, int to) {
    token* t        = malloc(sizeof(token));
    t->type         = type;
    t->from         = from;
    t->to           = to;
    t->string_value = NULL;
    
    return t;
}

token* token_create_int(int value, int from , int to) {
   token* t     = token_ctr(INTEGER, from, to);
   t->int_value = value;
   return t;
}

token* token_create_string(const char* value, int from, int to) {
    token* t = token_ctr(STRING, from , to);
    if(value) {
        size_t len = strlen(value);
        t->string_value = malloc(len+1);
        strcpy(t->string_value, value);
	t->string_value[len] = '\0';
    }

    return t;
}

token* token_create_float(float value, int from, int to) {
    token* t = token_ctr(FLOAT, from, to);
    t->float_value = value;
    return t;
}

token* token_create_name(const char* value, int from, int to) {
    token* t = token_create_string(value, from, to);
    t->type  = NAME;

    return t; 
}

token* token_create_operator(const char* value, int from, int to) {
    token* t = token_create_string(value, from, to);
    t->type  = OPERATOR;

    return t;
}

void token_free(token* t) {
  if(t->string_value) {
    free(t->string_value);
  }
  free(t);
}

void token_print(token* t) {
    
   switch(t->type) {
       case NAME:
            printf("Name ");
            printf("%s", t->string_value);
	    break;
       case STRING:
            printf("String ");
            printf("%s", t->string_value);
	    break;
       case OPERATOR:
            printf("Operator ");
            printf("%s", t->string_value);
	    break;
       case INTEGER:
            printf("Integer %d", t->int_value);
	    break;
       case FLOAT:
            printf("Float %f", t->float_value);
            break;
   }
   
   printf(" from %d to %d \n",t->from, t->to);
}

token_list* push_token(token_list* list_end, token* t) {
    token_list* wrapper = malloc(sizeof(token_list));
    wrapper->next       = NULL;
    wrapper->value      = t;
    list_end->next      = wrapper;
    return wrapper;
}

void token_list_free(token_list* head) {
    token_list* current = head;
    while(current != NULL) {
         token_free(current->value);
         token_list* temp = current;
         current          = temp->next;
         free(temp);  
    }
}

void print_token_list(token_list* head) {
   token_list* current = head;
   while(current != NULL) {
      if(current->value != NULL) {
          token_print(current->value);
      }
      current = current->next;
   }
}

int is_character(char c) {
   return ((c  >= 'a' && c <= 'z') || (c >= 'A' && c <='Z'));
}

int is_number(char c){
  return ( c >= '0' && c <= '9');
}

token_list* tokenize(const char* input) {
  int index, from, to;
  index = 0;
  char current;
  
  token_list* result = malloc(sizeof(token_list));
  token_list* head   = result;

  result->value = NULL;
    
  current = input[index];
  while(current != '\0') {
     //printf("%c", current);
     if(isspace(current)) {
         //printf("is space");
         index++;
         current = input[index];
     } else if(is_character(current)){
         from = index;
         char buffer[128];
         buffer[127] = '\0';
         int buffer_index = 0;
         buffer[buffer_index] = current;
         index++;
         buffer_index++;
         for(;;) {
//printf("loopisssa");
             current = input[index];
             if(is_character(current) || is_number(current)){
                  to = index;
                  buffer[buffer_index] = current;
                  buffer_index++;
                  index++;
             } else {
                 break;
             }   
         }
         token* t = token_create_name(buffer,from, to);
         head     = push_token(head, t);
     }else {
      printf("Ei tuettu merkki %c", current);
      break;
     }
  }

  return result; 
}

