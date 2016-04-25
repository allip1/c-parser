#ifndef PARSER_H
#define PARSER_H


enum token_type { NAME, INTEGER, OPERATOR, FLOAT, STRING };

struct token;
typedef struct token token;
struct token {
	enum token_type type;
	int             int_value;
        float      	float_value;
        char*      	string_value;
        int        	from;
        int        	to;
};

struct token_list;
typedef struct token_list token_list;
struct token_list {
      token*      value;
      token_list* next;
};


token* token_create_int(int value, int from , int to);
token* token_create_string(const char* value, int from, int to);
token* token_create_float(float value, int from , int to);
token* token_create_name(const char* value, int from, int to);
token* token_create_operator(const char* value, int from, int to);

void   token_free(token* t);
void   token_print(token* t);

token_list* tokenize(const char* input);
token_list* push_token(token_list* list_end, token* t);

void token_list_free(token_list* head);
void print_token_list(token_list* head);

#endif



