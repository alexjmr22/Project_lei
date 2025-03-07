/* START definitions section -- C code delimited by %{ ... %} and token declarations */

%{

#include <stdio.h>
#include "ast.h"
#include "semantics.h"

int yylex(void);
void yyerror(char *);

struct node *program;

%}
%union{
    char *lexeme;
    struct node *node;
}


%token INTEGER IF ELSE SEMICOLON VAR LPAR RPAR FLOAT32 BOOL STR COMMA FUNC LBRACE RBRACE ASSIGN FOR RETURN PRINT BLANKID PARSEINT CMDARGS LSQ RSQ OR AND 
LT GT EQ NE LE GE PLUS MINUS STAR DIV MOD NOT PACKAGE RESERVED
%token<lexeme> IDENTIFIER NATURAL DECIMAL STRLIT
%type<node> program parameters declarations vardeclaration funcdeclaration varspec type typerep funcbody varsandstatements
statement expr statementrep funcinvocation parseargs exprep 


%left LOW
%left OR
%left AND
%left LPAR RPAR LSQ RSQ LBRACE RBRACE IDENTIFIER
%left LT GT LE GE EQ NE ASSIGN COMMA
%left PLUS MINUS
%left STAR DIV MOD
%left HIGH



/* START grammar rules section -- BNF grammar */

%%

program: PACKAGE IDENTIFIER SEMICOLON declarations  {$$ = $4;}
            ;

declarations:                                       
            declarations vardeclaration SEMICOLON  {$$ = $1;
                                                    if($2){
                                                        correct($$, $2);
                                                    }
                                                    }
            |declarations funcdeclaration SEMICOLON {$$ = $1;
                                                     addchild($$,$2);
                                                    }
            |                                       {$$ = program = newnode(Program,NULL);}
            ;

vardeclaration: VAR varspec                         {$$ = $2;
                                                    }
              | VAR LPAR varspec SEMICOLON RPAR     {$$ = $3;
                                                    }
            ;



funcdeclaration: FUNC IDENTIFIER LPAR parameters RPAR type funcbody         {$$ = newnode(FuncDecl,NULL);
                                                                             struct node *fh = newnode(FuncHeader, NULL);
                                                                             addchild(fh, newnode(Identifier,$2));
                                                                             addchild(fh,$6);
                                                                             addchild(fh, $4);
                                                                             addchild($$, fh);
                                                                             addchild($$, $7);
                                                                            }
                 |FUNC IDENTIFIER LPAR parameters RPAR funcbody              {$$ = newnode(FuncDecl,NULL);
                                                                             struct node *fh = newnode(FuncHeader, NULL);
                                                                             addchild(fh, newnode(Identifier,$2));
                                                                             addchild(fh, $4);
                                                                             addchild($$, fh);
                                                                             addchild($$, $6);
                                                                            }
                 |FUNC IDENTIFIER LPAR RPAR type funcbody                    {$$ = newnode(FuncDecl,NULL);
                                                                             struct node *fh = newnode(FuncHeader, NULL);
                                                                             addchild(fh, newnode(Identifier,$2));
                                                                             addchild(fh,$5);
                                                                             addchild(fh, newnode(FuncParams, NULL));
                                                                             addchild($$, fh);
                                                                             addchild($$, $6);
                                                                            }
                 |FUNC IDENTIFIER LPAR RPAR funcbody                         {$$ = newnode(FuncDecl,NULL);
                                                                             struct node *fh = newnode(FuncHeader, NULL);
                                                                             addchild(fh, newnode(Identifier,$2));
                                                                             addchild(fh, newnode(FuncParams, NULL));
                                                                             addchild($$, fh);
                                                                             addchild($$, $5);
                                                                            }

            ;

funcbody: LBRACE varsandstatements RBRACE                   {$$ = $2;}
            ;

varsandstatements:varsandstatements vardeclaration SEMICOLON   {$$ = $1;
                                                                if($2){
                                                                    correct($$,$2);
                                                                }
                                                                }
                    |varsandstatements statement SEMICOLON      {
                                                                $$ = $1;
                                                                addchild($$, $2);
                                                                }
                    |varsandstatements SEMICOLON                {$$ = $1;}
                    |                                           {$$ = newnode(FuncBody, NULL);}
            ;


// tirar os $$ = $? -----------------------------------
// COMEÇA----------------------------------------------------------------------------------------------
statement:  IDENTIFIER ASSIGN expr                  {$$ = newnode(Assign,NULL); //1
                                                     addchild($$, newnode(Identifier, $1));
                                                     addchild($$, $3);
                                                    }                              
            |LBRACE statementrep RBRACE              { if(childcount($2) == 1){ $$ = semblock($2);} //2
                                                       else if(childcount($2)!= 0){ $$ = $2;}
                                                       else{$$ = newnode(Anula, NULL);}
                                                     }
            |LBRACE RBRACE                          {$$ = newnode(Anula,NULL);}//3

            |IF expr LBRACE statementrep RBRACE     {$$ = newnode(If, NULL);//4
                                                         addchild($$, $2);
                                                         addchild($$, $4);
                                                         addchild($$, newnode(Block, NULL));
                                                    }
            |IF expr LBRACE RBRACE                  {$$ = newnode(If,NULL);//5
                                                     addchild($$,$2);
                                                     addchild($$,newnode(Block, NULL));
                                                     addchild($$,newnode(Block, NULL));
                                                    }
            |IF expr LBRACE statementrep RBRACE ELSE LBRACE statementrep RBRACE         {$$ = newnode(If,NULL);//6
                                                                                         addchild($$, $2);
                                                                                         addchild($$, $4);
                                                                                         addchild($$, $8);
                                                                                        }
            |IF expr LBRACE RBRACE ELSE LBRACE statementrep RBRACE                      {$$ = newnode(If,NULL);//7
                                                                                         addchild($$, $2);
                                                                                         addchild($$,newnode(Block, NULL));
                                                                                         addchild($$, $7);
                                                                                        }
            |IF expr LBRACE statementrep RBRACE ELSE LBRACE RBRACE                      {$$ = newnode(If,NULL);//8
                                                                                         addchild($$, $2);
                                                                                         addchild($$, $4);
                                                                                         addchild($$, newnode(Block,NULL));
                                                                                        }
            |IF expr LBRACE RBRACE ELSE LBRACE RBRACE                                   {$$ = newnode(If,NULL);//9
                                                                                         addchild($$, $2);
                                                                                         addchild($$,newnode(Block, NULL));
                                                                                         addchild($$, newnode(Block, NULL));
                                                                                        }
            |FOR LBRACE statementrep RBRACE          {$$ = newnode(For, NULL);//10
                                                     addchild($$, $3);
                                                    }
            |FOR LBRACE RBRACE                      {$$ = newnode(For,NULL);//11
                                                     addchild($$,newnode(Block, NULL));
                                                    }
            |FOR expr LBRACE statementrep RBRACE     {$$ = newnode(For, NULL);//12
                                                     addchild($$,$2);
                                                     addchild($$, $4);
                                                    }
            |FOR expr LBRACE RBRACE                 {$$ = newnode(For, NULL);//13
                                                     addchild($$,$2);
                                                     addchild($$, newnode(Block, NULL));
                                                    }
            |RETURN                                  {$$ = newnode(Return, NULL);//14
                                                    }
            |RETURN expr                             {$$ = newnode(Return, NULL);//15
                                                     addchild($$, $2);
                                                    }
            |funcinvocation                          {$$ = $1;//16
                                                    }
            |parseargs                               {$$ = $1;//17
                                                    }
            |PRINT LPAR expr RPAR                    {$$ = newnode(Print, NULL);//18
                                                     addchild($$, $3);
                                                    }
            |PRINT LPAR STRLIT RPAR                  {$$ = newnode(Print, NULL);//19
                                                     addchild($$, newnode(Strlit, $3));
                                                    }
            |error                                  {}//20
            ;



statementrep: statementrep statement SEMICOLON                  {$$ = $1;
                                                                 addchild($$, $2);
                                                                }
                |statement SEMICOLON                {$$ = newnode(Block, NULL);
                                                     addchild($$, $1);
                                                    }
            ;

//ACABA-------------------------------------------------------------------------------------------------

funcinvocation: IDENTIFIER LPAR RPAR                {$$ = newnode(Call, NULL);
                                                     addchild($$, newnode(Identifier, $1));
                                                    }
                                                    /*FAZER FUNÇAO PARA INSERIR TUDO DIREITO*/
                |IDENTIFIER LPAR exprep RPAR        {$$ = newnode(Call, NULL);
                                                     addchild($$, newnode(Identifier, $1));
                                                     struct node_list *semCall = $3->children->next;
                                                     while(semCall!=NULL){
                                                        addchild($$,semCall->node);
                                                        semCall = semCall->next;
                                                     }
                                                    }
                |IDENTIFIER LPAR error RPAR         {}
            ;

exprep:                                             
        exprep COMMA expr                           {$$ = $1;
                                                     addchild($$,$3);
                                                    }
        |expr                                       {$$ = newnode(Call, NULL);
                                                     addchild($$,$1);
                                                    }
            ;

parseargs:  IDENTIFIER COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ expr RSQ RPAR   {$$ = newnode(ParseArgs, NULL);
                                                                                       addchild($$, newnode(Identifier, $1));
                                                                                       addchild($$, $9);
                                                                                      }
            |IDENTIFIER COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ error RSQ RPAR   {}
            ;

expr:  expr OR expr                           { $$ = newnode(Or, NULL);
                                               addchild($$,$1);
                                               addchild($$, $3);
                                              }
       |expr AND expr                         {$$ = newnode(And, NULL);
                                               addchild($$, $1);
                                               addchild($$, $3);
                                              }
       |expr LT expr                          {$$ = newnode(Lt, NULL);
                                               addchild($$, $1);
                                               addchild($$, $3);
                                              }
       |expr GT expr                          {$$ = newnode(Gt, NULL);
                                               addchild($$, $1);
                                               addchild($$, $3);
                                              }
       |expr EQ expr                          {$$ = newnode(Eq, NULL);
                                               addchild($$, $1);
                                               addchild($$, $3);
                                              }
       |expr NE expr                          {$$ = newnode(Ne, NULL);
                                               addchild($$, $1);
                                               addchild($$, $3);
                                              }
       |expr LE expr                          {$$ = newnode(Le, NULL);
                                               addchild($$, $1);
                                               addchild($$, $3);
                                              }
       |expr GE expr                          {$$ = newnode(Ge, NULL);
                                               addchild($$, $1);
                                               addchild($$, $3);
                                              }
       |expr PLUS expr                        {$$ = newnode(Add, NULL);
                                               addchild($$, $1);
                                               addchild($$, $3);
                                              }
       |expr MINUS expr                       {$$ = newnode(Sub, NULL);
                                               addchild($$, $1);
                                               addchild($$, $3);
                                              }
       |expr STAR expr                        {$$ = newnode(Mul, NULL);
                                               addchild($$, $1);
                                               addchild($$, $3);
                                              }
       |expr DIV expr                         {$$ = newnode(Div, NULL);
                                               addchild($$, $1);
                                               addchild($$, $3);
                                              }
       |expr MOD expr                         {$$ = newnode(Mod, NULL);
                                               addchild($$, $1);
                                               addchild($$, $3);
                                              }
       |NOT expr %prec HIGH                   {$$ = newnode(Not,NULL);
                                               addchild($$, $2);
                                              }
       |MINUS expr %prec HIGH                 {$$ = newnode(Minus,NULL);
                                               addchild($$, $2);
                                              }
       |PLUS expr %prec HIGH                  {$$ = newnode(Plus,NULL);
                                               addchild($$, $2);
                                              }
       |NATURAL                               {$$ = newnode(Natural, $1);}
       |DECIMAL                               {$$ = newnode(Decimal, $1);}
       |IDENTIFIER                            {$$ = newnode(Identifier, $1);}
       |funcinvocation                        {$$ = $1;}
        |LPAR expr RPAR                        {$$ = $2;
                                              }
        |LPAR error RPAR                        {}
            ;


varspec: IDENTIFIER typerep type                    {$$ = newnode(VarDecl, NULL);
                                                     addchild($$, $3);
                                                     addchild($$, newnode(Identifier,$1));
                                                     var($$, $2, $3);
                                                    }
            ;

typerep:                                           
        typerep COMMA IDENTIFIER                  {$$ = $1;
                                                     addchild($$, newnode(Identifier, $3));
                                                    }
        |                                            {$$ = newnode(VarDecl,NULL);}
            ;

type: INTEGER                       {$$ = newnode(Int,NULL);
                                    }
    | FLOAT32                       {$$ =  newnode(Float32,NULL);
                                    }
    | BOOL                          {$$ =  newnode(Bool,NULL);
                                    }
    | STR                        {$$ = newnode(String,NULL);
                                    }
            ;

parameters: IDENTIFIER type              {$$ = newnode(FuncParams,NULL);
                                          struct node *ParamDec = newnode(ParamDecl, NULL);
                                          addchild(ParamDec, $2);
                                          addchild(ParamDec, newnode(Identifier, $1));
                                          addchild($$, ParamDec);
                                            }
        |parameters COMMA IDENTIFIER type   {$$ = $1;
                                           struct node *ParamDec = newnode(ParamDecl, NULL);
                                           addchild(ParamDec, $4);
                                           addchild(ParamDec, newnode(Identifier,$3));
                                           addchild($$,ParamDec);
                                          }
                                          
    ;


/* PASSA O NO REPARAM PARA CIMA E EM CIMA LIGA A RAIZ AO NO PARAM E N AOS PARAMETROS DELE*/


%%

/* START subroutines section */

// all needed functions are collected in the .l and ast.* files
