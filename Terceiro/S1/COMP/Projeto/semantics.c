#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantics.h"

int semantic_errors = 0;

struct global *global;

// Função para verificar se uma função está corretamente declarada
void check_function(struct node *function) {
    if (!function) {
        printf("Invalid function node\n");
        semantic_errors++;
        return;
    }

    // Saltar o FuncHeader
    struct node *header = getchild(function, 0);
    if (!header) {
        printf("Invalid header node in function-22\n");
        semantic_errors++;
        return;
    }

    // Identifier do FuncHeader
    struct node *id = getchild(header, 0);
    if (!id || !id->token) {
        printf("Invalid identifier in function header-30\n");
        semantic_errors++;
        return;
    }

    // Type do FuncHeader
    struct node *type_node = getchild(header, 1);
    if (!type_node) {
        printf("Invalid type node in function header-38\n");
        semantic_errors++;
        return;
    }
    enum type type = category_type(type_node->category);

    if (search_symbol_global(global, id->token) == NULL) {
        if (!insert_symbol_global(global, id->token, type, function)) {
            printf("Failed to insert global symbol46\n");
            semantic_errors++;
        }
    } else {
        printf("Identifier %s already declare50d\n", id->token);
        semantic_errors++;
    }

    // Processar os parâmetros da função
    struct node *params = getchild(header, 1); // ParamDeclList ou similar
    if (params) {
        struct node_list *param_list = params->children;
        while (param_list) {
            struct node *param = param_list->node;
            if (param) {
                struct node *param_id = getchild(param, 0);
                struct node *param_type_node = getchild(param, 1);
                if (param_id && param_type_node) {
                    enum type param_type = category_type(param_type_node->category);
                    if (!insert_symbol(global->lista, param_id->token, param_type, param)) {
                        printf("Failed to insert parameter %s\n", param_id->token); // Debug para inserção
                        semantic_errors++;
                    }
                } else {
                    printf("Invalid parameter declaration70\n");
                    semantic_errors++;
                }
            }
            param_list = param_list->next;
        }
    } else {
        printf("No parameters found for function %s\n", id->token); // Debug para ausência de parâmetros
    }

    // Processar as variáveis locais no corpo da função
    struct node *body = getchild(function, 1);
    if (body) {
        struct node_list *statements = body->children;
        while (statements) {
            struct node *statement = statements->node;
            if (statement && (statement->category == VarDecl)) {
                struct node *var_id = getchild(statement, 0);
                struct node *var_type_node = getchild(statement, 1);
                if (var_id && var_type_node) {
                    enum type var_type = category_type(var_type_node->category);
                    if (!insert_symbol(global->lista, var_id->token, var_type, statement)) {
                        printf("Failed to insert variable %s\n", var_id->token); // Debug para inserção
                        semantic_errors++;
                    }
                } else {
                    printf("Invalid variable declaration96\n");
                    semantic_errors++;
                }
            }
            statements = statements->next;
        }
    }
}

// Análise semântica a partir do nó raiz da AST
int check_program(struct node *program) {
    if (!program) {
        printf("Invalid program node108\n");
        return -1;
    }

    // Inicializar tabela global
    global = (struct global *)malloc(sizeof(struct global));
    if (!global) {
        printf("Memory allocation failed for global symbol table\n");
        return -1;
    }
    global->next = NULL;
    global->identifier = NULL;
    global->type = no_type;
    global->node = NULL;
    global->lista = (struct symbol_list *)malloc(sizeof(struct symbol_list));
    if (!global->lista) {
        printf("Memory allocation failed for global symbol list\n");
        free(global);
        return -1;
    }
    global->lista->node = NULL;
    global->lista->identifier = NULL;
    global->lista->type = no_type;
    global->lista->next = NULL;

    // Percorrer os filhos do programa
    struct node_list *child = program->children;
    while (child=child->next) {
        if (child->node) {
            check_function(child->node);
        } else {
            printf("Invalid child node in program139\n");
            semantic_errors++;
        }
    }
    return semantic_errors;
}

// Inserir um novo símbolo na lista, a menos que ele já esteja presente
struct symbol_list *insert_symbol(struct symbol_list *table, char *identifier, enum type type, struct node *node) {
    if (!table || !identifier || !node) {
        printf("Invalid parameters for insert_symbol150\n");
        return NULL;
    }

    struct symbol_list *new = (struct symbol_list *)malloc(sizeof(struct symbol_list));
    if (!new) {
        printf("Memory allocation failed for new symbol\n");
        return NULL;
    }

    new->identifier = strdup(identifier);
    new->type = type;
    new->node = node;
    new->next = NULL;

    struct symbol_list *symbol = table;
    while (symbol) {
        if (strcmp(symbol->identifier, identifier) == 0) {
            printf("Symbol %s already exists\n", identifier);
            free(new->identifier);
            free(new);
            return NULL;           /* Symbol already inserted */
        }
        if (symbol->next == NULL) {
            symbol->next = new;    /* Insert new symbol at the tail of the list */
            return new;
        }
        symbol = symbol->next;
    }
    return new;
}

// Procurar um símbolo pelo identificador
struct symbol_list *search_symbol(struct symbol_list *table, char *identifier) {
    if (!table || !identifier) return NULL;

    struct symbol_list *symbol = table;
    while (symbol) {
        if (symbol->identifier && strcmp(symbol->identifier, identifier) == 0)
            return symbol;
        symbol = symbol->next;
    }
    return NULL;
}

// Inserir um símbolo na tabela global
struct global *insert_symbol_global(struct global *table, char *identifier, enum type type, struct node *node) {
    if (!table || !identifier || !node) {
        printf("Invalid parameters for insert_symbol_global198n");
        return NULL;
    }

    struct global *new = (struct global *)malloc(sizeof(struct global));
    if (!new) {
        printf("Memory allocation failed for new global symbol\n");
        return NULL;
    }

    new->next = NULL;
    new->identifier = strdup(identifier);
    new->type = type;
    new->node = node;
    new->lista = (struct symbol_list *)malloc(sizeof(struct symbol_list));
    if (!new->lista) {
        printf("Memory allocation failed for global symbol list\n");
        free(new->identifier);
        free(new);
        return NULL;
    }
    new->lista->node = NULL;
    new->lista->identifier = NULL;
    new->lista->type = no_type;
    new->lista->next = NULL;

    struct global *symbol = table;
    while (symbol) {
        if (symbol->next == NULL) {
            symbol->next = new;    /* Insert new symbol at the tail of the list */
            return new;
        } else if (strcmp(symbol->next->identifier, identifier) == 0) {
            printf("Global symbol %s already exists\n", identifier);
            free(new->lista);
            free(new->identifier);
            free(new);
            return NULL;           /* Symbol already inserted */
        }
        symbol = symbol->next;
    }
    return new;
}

// Procurar um símbolo na tabela global
struct global *search_symbol_global(struct global *table, char *identifier) {
    if (!table || !identifier) return NULL;

    struct global *symbol = table;
    while (symbol) {
        if (symbol->identifier && strcmp(symbol->identifier, identifier) == 0)
            return symbol;
        symbol = symbol->next;
    }
    return NULL;
}

void show_symbol_table() {
    if (!global) {
        printf("Global symbol table is empty\n");
        return;
    }

    struct global *inicio = global;
    printf("===== Global Symbol Table =====\n");

    // Imprimir a tabela global
    while ((inicio = inicio->next)) {
        if (!inicio->identifier || !inicio->node) continue;

        struct node *header = getchild(inicio->node, 0);
        struct node *funcparams = getchild(header, 1);

        // Nome da função e seus parâmetros
        printf("%s\t(", inicio->identifier);
        if(funcparams->category == FuncParams){
            struct node* decl;
            if(funcparams->children != NULL){
                struct node_list *declarations = funcparams->children;
                int flag = 0;
                while((declarations = declarations->next) !=NULL){
                    if(flag>0)printf(",");
                    flag=1;
                    decl = getchild(declarations->node,0);
                    printf("%s",type_name(category_type(decl->category)));
                }
                printf(")");
            }
            printf("\t%s\n",type_name(inicio->type));
        }else{
            funcparams = getchild(header, 2);
            struct node* decl;
            if(funcparams->children != NULL){
                struct node_list *declarations = funcparams->children;
                int flag = 0;
                while((declarations = declarations->next) !=NULL){
                    if(flag>0)printf(",");
                    flag=1;
                    decl = getchild(declarations->node,0);
                    printf("%s",type_name(category_type(decl->category)));
                }
                printf(")");
            }
            printf("\t%s\n",type_name(inicio->type));
        }
    }

    // Mostrar tabelas específicas para cada função
    inicio = global;
    while ((inicio = inicio->next)) {
        if (!inicio->identifier || !inicio->node) continue;

        struct node *header = getchild(inicio->node, 0);
        struct node *funcparams = getchild(header, 1);
        struct node *body = getchild(inicio->node, 1);

        printf("\n===== Function %s(", inicio->identifier);

        struct node_list *params = funcparams ? funcparams->children : NULL;
        int flag = 0;
        while (params) {
            struct node *param = params->node;
            if (param) {
                if (flag > 0) printf(", ");
                printf("%s", type_name(category_type(param->category))); // Corrigido: impressão de tipos dos parâmetros
                flag = 1;
            }
            params = params->next;
        }
        printf(") Symbol Table =====\n");

        // Símbolo de retorno
        printf("return\t%s\n", type_name(inicio->type));

        // Símbolos do corpo da função
        if (body) {
            struct node_list *statements = body->children;
            while (statements) {
                struct node *statement = statements->node;
                if (!statement) {
                    statements = statements->next;
                    continue;
                }

                if (statement->category == ParamDecl || statement->category == VarDecl) {
                    struct node *id = getchild(statement, 0);
                    struct node *type_node = getchild(statement, 1);
                    if (id && type_node) {
                        enum type type = category_type(type_node->category);
                        const char *param_flag = (statement->category == ParamDecl) ? "\tparam" : "";
                        printf("%s\t%s%s\n", id->token, type_name(type), param_flag);
                    }
                }
                statements = statements->next;
            }
        }

        printf("\n");
    }
}
