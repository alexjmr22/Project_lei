#ifndef _SEMANTICS_H
#define _SEMANTICS_H

#include "ast.h"


struct global{
	struct global *next;
	char *identifier;
	enum type type;
	struct node *node;
	struct symbol_list *lista;
};

struct symbol_list {
	char *identifier;
	enum type type;
	struct node *node;
	struct symbol_list *next;
};

char *getParamstype(struct node *funcdecl);
struct global *insert_symbol_global(struct global *table, char *identifier, enum type type, struct node *node);
struct global *search_symbol_global(struct global *table, char *identifier);
struct symbol_list *insert_symbol(struct symbol_list *symbol_table, char *identifier, enum type type, struct node *node);
struct symbol_list *search_symbol(struct symbol_list *symbol_table, char *identifier);
void show_symbol_table();
int check_program(struct node *program);

#endif