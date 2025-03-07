#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "semantics.h"

// create a node of a given category with a given lexical symbol
struct node *newnode(enum category category, char *token)
{
    struct node *new = malloc(sizeof(struct node));
    new->category = category;
    new->token = token;
    new->children = malloc(sizeof(struct node_list));
    new->children->node = NULL;
    new->children->next = NULL;
    return new;
}

// append a node to the list of children of the parent node
void addchild(struct node *parent, struct node *child)
{
    struct node_list *new = malloc(sizeof(struct node_list));
    new->node = child;
    new->next = NULL;
    struct node_list *children = parent->children;
    while (children->next != NULL)
        children = children->next;
    children->next = new;
}

void show(struct node *node, int depth)
{
    const char *categoryString[66] = {"Program", "Function", "Parameters", "Parameter", "Arguments", "Int", "Float32", "Bool", "String", "Double", "Identifier", "Natural", "Decimal", "Call", "If", "Add",
                                      "Sub", "Mul", "Div", "Declarations", "VarDecl", "FuncDecl", "Var", "Type", "VarSpec", "Func", "FuncBody", "VarsAndStatements", "Lpar", "Rpar", "Lbrace", "Rbrace",
                                      "Statement", "Assign", "For", "Return", "Print", "StrLit", "ParseArgs", "Blankid", "Parseint", "Lsq", "Rsq", "Cmdargs", "Funcinvocation", "Or", "And", "Expr", "Lt", "Gt",
                                      "Eq", "Ne", "Le", "Ge", "Plus", "Minus", "Star", "Mod", "Not", "Else", "Error", "FuncHeader", "ParamDecl", "FuncParams", "Block", "Anula"};

    if (node != NULL)
    {
        if(node->category!=Anula){
            for (int traces = 0; traces < depth; traces++)
            {
                printf("..");
            }

            printf("%s", categoryString[node->category]);
            if (node->token != NULL)
            {
                printf("(%s)", node->token);
            }
            printf("\n");
        }
            struct node_list *child = node->children;
            while (child != NULL)
            {
                show(child->node, depth + 1);
                child = child->next;
            }
        
    }
}

int childcount(struct node *node)
{
    int count = 0;
    struct node_list *child = node->children;
    while (child->next != NULL)
    {
        if(child->next->node->category!=Anula){
            count++;
        }
        
        child = child->next;
    }
    return count;
}

struct node *semblock(struct node *parent)
{
    struct node_list *child = parent->children->next;
    free(parent);
    if (child->node->category!=Anula)
    {
        return child->node;
    }else{
        while(child->node->category==Anula){
            child = child->next;
        }
        return child->node;
    }
    return NULL;
}

void var(struct node *parent, struct node *childr, struct node *tipo)
{
    struct node_list *child = childr->children->next;
    free(childr);
    if (child != NULL)
    {
        while (child != NULL)
        {
            addchild(parent, tipo);
            addchild(parent, child->node);
            child = child->next;
        }
    }
}
void correct(struct node *parent, struct node *childr)
{
    struct node_list *child = childr->children->next;
    free(childr);
    if (child != NULL)
    {
        while (child != NULL)
        {
            struct node *decl = newnode(VarDecl, NULL);
            addchild(decl, child->node);
            child = child->next;
            addchild(decl, child->node);
            addchild(parent, decl);
            child = child->next;
        }
    }
}

struct node* allNull(struct node* parent){
    struct node* block = newnode(Block,NULL);
    struct node_list* child = parent->children->next;
    while(child!=NULL){
        if(child->node!=NULL){
            addchild(block,child->node);
        }
    }
    if(block->children!=NULL){
        free(parent);
        return block;
    }else{
        return parent;
    }
}
struct node *getchild(struct node *parent, int position) {
    struct node_list *children = parent->children;
    
    while((children = children->next) != NULL)
        if(position-- == 0)
            return children->node;
    return NULL;
}