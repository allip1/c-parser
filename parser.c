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
         if(current->value != NULL) {
         token_free(current->value);
         }
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

typedef int (*continuationf)(char c);

int read_to_buffer(int index,int* buffer_index, char* buffer, continuationf f, const char* input){
   int to = index;
   char current = input[index];
   buffer[*buffer_index] = current;
   index++;
   (*buffer_index)++;
   for(;;) {
       current = input[index];
       if(f(current)) {
           to = index;
           buffer[(*buffer_index)] = current;
           (*buffer_index)++;
           index++;
       } else {
           break;
       }
   }
   return to;
}

int is_name_char(char c) {
   return (is_number(c) || is_character(c));
}

int is_not_quotationmark(char c) {
   return !(c == '\"');
}

int is_in_str(char c, const char* str) {
  while(*str != '\0') {
    if((*str) == c) { return 1; }
    str++;
  }
  return 0;
}

int is_double_operator(char c) {
  return is_in_str(c,"><=&|+-");
}

token_list* tokenize(const char* input) {
  int index, from, to;
  index = 0;
  char current;
  char buffer[128]; 
  token_list* result = malloc(sizeof(token_list));
  token_list* head   = result;

  result->value = NULL;
    
  while((current = input[index]) != '\0') {
    
     if(isspace(current)) {
         index++;
         current = input[index];
         continue;
     }

     memset(buffer, 0, 128);
     buffer[127]      = '\0';
     from             = index;
     int buffer_index = 0;

    if(is_character(current)){
         to       = read_to_buffer(index, &buffer_index, buffer, is_name_char, input);
         index    = to + 1; 
         token* t = token_create_name(buffer,from, to);
         head     = push_token(head, t);
     }else if(is_number(current)) {
        to        = read_to_buffer(index, &buffer_index, buffer, is_number, input);
        index     = to + 1;
        current   = input[index];
        if(current == '.') {
             index++;
             buffer[buffer_index] = '.';
             buffer_index++;
             current = input[index];
             if(!is_number(current)) {
                printf("Expected number after \".\"");
                break;
             }
             to          = read_to_buffer(index, &buffer_index, buffer, is_number, input);
             index       = to + 1;
             float value = atof(buffer);
             token* t    = token_create_float(value, from, to);
             head        = push_token(head, t); 
        } else {
        int value = atoi(buffer);
        token* t  = token_create_int(value, from, to);
        head      = push_token(head, t); 
        } 

	
     } else if (current == '\"') {
          index++;
          to      = read_to_buffer(index, &buffer_index, buffer, is_not_quotationmark, input);           
	  index   = to +2;
          token* t= token_create_string(buffer, from, to);
          head    = push_token(head, t);
           
       } else if(is_double_operator(current)) {
          to      = read_to_buffer(index, &buffer_index, buffer, is_double_operator,input);    
          index   = to + 1;
          token* t= token_create_operator(buffer, from, to);
          head    = push_token(head, t);
       } else { //yksittainen operaattori 
         to = index;
         buffer[0] = current;
         buffer[1] = '\0';
         token* t  = token_create_operator(buffer, from, to);
         head      = push_token(head, t);
         index++;
     }
  }

  return result; 
}

