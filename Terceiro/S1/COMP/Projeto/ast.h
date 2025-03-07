#ifndef _AST_H
#define _AST_H


enum category { Program, Function, Parameters, Parameter, Arguments, Int, Float32, Bool, String, Double, Identifier, Natural, Decimal, Call, If, Add, Sub, Mul, Div, Declarations, VarDecl,
FuncDecl, Var, Type, VarSpec, Func, FuncBody, VarsAndStatements, Lpar, Rpar, Lbrace, Rbrace, Statement, Assign, For, Return, Print, Strlit, ParseArgs, Blankid, Parseint,
Lsq, Rsq, Cmdargs, Funcinvocation, Or, And, Expr, Lt, Gt, Eq, Ne, Le, Ge, Plus, Minus, Star, Mod, Not, Else, Error, FuncHeader, ParamDecl, FuncParams, Block, Anula};

enum type { integer_type, float32_type, bool_type, strlit_type, no_type};
#define type_name(type) (type == integer_type ? "int" :\
(type == float32_type ? "float32":\
(type == bool_type ? "bool":\
(type == strlit_type ? "string" : "none"))))
#define category_type(category) (category == Int ? integer_type : \
(category == Float32 ? float32_type:\
(category == Bool ? bool_type:\
(category == Strlit ? strlit_type : no_type))))

struct node {
    enum category category;
    char *token;
    enum type type;
    struct node_list *children;
    int token_column;
    int token_line;
};

struct node_list {
    struct node *node;
    struct node_list *next;
};

struct node *newnode(enum category category, char *token);
void addchild(struct node *parent, struct node *child);
void show(struct node *node, int depth);
int childcount(struct node *node);
struct node *semblock(struct node *parent);
void var(struct node *parent, struct node *childr, struct node *tipo);
void correct(struct node *parent, struct node *childr);
struct node* allNull(struct node* parent);
struct node *getchild(struct node *parent, int position);

#endif