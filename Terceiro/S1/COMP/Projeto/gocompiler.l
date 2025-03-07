/*Joao Barraca 2022244716
Alexandre Rodrigues  2022249408
*/


%{
#include <stdio.h>
#include "y.tab.h"
#include "ast.h"
#include "semantics.h"
// macro YY_USER_ACTION, which updates 'lex_column', is executed before every action
#define YY_USER_ACTION { syn_line = line; syn_column = colunas; colunas += yyleng;  }
#define PASS_TOKEN() yylval.lexeme = strdup(yytext);

int yyparse(void);
void yyerror(char *);

// prode ser removido???????????????????????????
int syn_line = -1, syn_column = -1;
int errors = 0;

extern struct node *program;
extern int yydebug;
int line = 1;
int colunas = 1;
char buffer [1019];
char buffer2 [1024];

int erroescape = 0;
int i=0;
int val=0;
int espace = 0;
int linhacomment = 0;
int colunacomment = 0;
int linhastring = 0;
int colunastring = 0;
int TokenOn=0;
%}


%X COMMENT 
%X COMMENT2
%X STRING


digit   [0-9]
char    [a-zA-Z]
hexa    [a-fA-F]
octal   [0-7]
ignore  [\t\r]
nonoctal [8-9]
space [ ]

%%

"package"                    { if(TokenOn)printf("PACKAGE\n");   val=0;return PACKAGE;}
"return"                     { if(TokenOn)printf("RETURN\n");   val=1;return RETURN;}
"else"                       { if(TokenOn)printf("ELSE\n");   val=0;return ELSE;}
"for"                        { if(TokenOn)printf("FOR\n");   val=0;return FOR;}
"if"                         { if(TokenOn)printf("IF\n");   val=0;return IF;}
"var"                        { if(TokenOn)printf("VAR\n");   val=0;return VAR;}
"int"                        { if(TokenOn)printf("INTEGER\n");   val=0;return INTEGER;}
"float32"                    { if(TokenOn)printf("FLOAT32\n");   val=0;return FLOAT32;}
"bool"                       { if(TokenOn)printf("BOOL\n");   val=0;return BOOL;}
"string"                     { if(TokenOn)printf("STRING\n");   val=0;return STR;}
"fmt.Println"                { if(TokenOn)printf("PRINT\n");   val=0;return PRINT;}
"strconv.Atoi"               { if(TokenOn)printf("PARSEINT\n");   val=0;return PARSEINT;}
"func"                       { if(TokenOn)printf("FUNC\n");   val=0;return FUNC;}
"os.Args"                    { if(TokenOn)printf("CMDARGS\n");   val=0;return CMDARGS;}

"break"                      { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;   val=0;}
"default"                    { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;   val=0;}
"interface"                  { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;  val=0;}
"chan"                       { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;  val=0;}
"type"                       { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;  val=0;}
"select"                     { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;  val=0;}
"struct"                     { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;  val=0;}
"fallthrough"                { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;  val=0;}
"switch"                     { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;   val=0;}
"goto"                       { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;  val=0;}
"go"                         { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;  val=0;}
"map"                        { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;   val=0;}
"defer"                      { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;   val=0;}
"case"                       { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;   val=0;}
"const"                      { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;  val=0;}
"range"                      { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;  val=0;}
"continue"                   { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;  val=0;}
"import"                     { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;  val=0;}


"0"("x"|"X")({digit}|{hexa})+ { if(TokenOn)printf("NATURAL(%s)\n",yytext);    val=1;PASS_TOKEN();buffer2[0] = '\0'; return NATURAL;}

"0"{octal}*                   { if(TokenOn)printf("NATURAL(%s)\n",yytext);   val=1;PASS_TOKEN(); buffer2[0] = '\0'; return NATURAL;}
"0"({octal}*{nonoctal}+{octal}*)* { printf("Line %d, column %d: invalid octal constant (%s)\n", line, colunas, yytext); val=1; buffer2[0] = '\0';}

{digit}+                      { if(TokenOn)printf("NATURAL(%s)\n", yytext);   val=1;PASS_TOKEN(); buffer2[0] = '\0'; return NATURAL;}

{digit}+([eE][+-]?{digit}+)    { if(TokenOn)printf("DECIMAL(%s)\n", yytext);   val=1;PASS_TOKEN(); buffer2[0] = '\0'; return DECIMAL;}
{digit}*"."{digit}+            { if(TokenOn)printf("DECIMAL(%s)\n", yytext);   val=1;PASS_TOKEN(); buffer2[0] = '\0'; return DECIMAL;}
{digit}*("."{digit}+)([eE][+-]?{digit}+)? { if(TokenOn)printf("DECIMAL(%s)\n", yytext);   val=1; PASS_TOKEN(); buffer2[0] = '\0'; return DECIMAL;}
{digit}+("."{digit}*)?([eE][+-]?{digit}+)? { if(TokenOn)printf("DECIMAL(%s)\n", yytext);   val=1; PASS_TOKEN(); buffer2[0] = '\0'; return DECIMAL;}

{char}({char}|_|{digit})*      { if(TokenOn)printf("IDENTIFIER(%s)\n", yytext);   val=1;PASS_TOKEN(); buffer2[0] = '\0'; return IDENTIFIER;}
"_"({char}|_|{digit})+         { if(TokenOn)printf("IDENTIFIER(%s)\n", yytext);   val=1;PASS_TOKEN(); buffer2[0] = '\0'; return IDENTIFIER;}

{ignore}                       ;
{space}                        ;

"\n"                           { colunas = 1;line++;
                                if(val == 1) {
                                    if(TokenOn){
                                        printf("SEMICOLON\n");
                                    }
                                    val = 0;
                                    return SEMICOLON;
                                    }
                                }

"++"                           { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED;  val=0;}
"--"                           { if(TokenOn)printf("RESERVED(%s)\n",yytext); PASS_TOKEN();return RESERVED; val=0;}
"("                            { if(TokenOn)printf("LPAR\n");  val=0;return LPAR;}
"_"                            { if(TokenOn)printf("BLANKID\n");  val=0;return BLANKID;}
")"                            { if(TokenOn)printf("RPAR\n");  val=1;return RPAR;}
"{"                            { if(TokenOn)printf("LBRACE\n");  val=0;return LBRACE;}
"}"                            { if(TokenOn)printf("RBRACE\n");val=1; return RBRACE;}
"["                            { if(TokenOn)printf("LSQ\n");  val=0;return LSQ;}
"]"                            { if(TokenOn)printf("RSQ\n");  val=1;return RSQ;}
";"                            { if(TokenOn){printf("SEMICOLON\n");}  val=0;return SEMICOLON;}
","                            { if(TokenOn)printf("COMMA\n");  val=0;return COMMA;}
"="                            { if(TokenOn)printf("ASSIGN\n");  val=0;return ASSIGN;}
"*"                            { if(TokenOn)printf("STAR\n");  val=0;return STAR;}
"/"                            { if(TokenOn)printf("DIV\n");  val=0;return DIV;}
"-"                            { if(TokenOn)printf("MINUS\n");  val=0;return MINUS;}
"+"                            { if(TokenOn)printf("PLUS\n");  val=0;return PLUS;}
"=="                           { if(TokenOn)printf("EQ\n");   val=0;return EQ;}
">="                           { if(TokenOn)printf("GE\n");   val=0;return GE;}
">"                            { if(TokenOn)printf("GT\n");  val=0;return GT;}
"<="                           { if(TokenOn)printf("LE\n");   val=0;return LE;}
"<"                            { if(TokenOn)printf("LT\n");  val=0;return LT;}
"%"                            { if(TokenOn)printf("MOD\n");  val=0;return MOD;}
"!="                           { if(TokenOn)printf("NE\n");   val=0;return NE;}
"!"                            { if(TokenOn)printf("NOT\n");  val=0;return NOT;}
"&&"                           { if(TokenOn)printf("AND\n");  val=0; return AND;}
"||"                           { if(TokenOn)printf("OR\n");   val=0;return OR;}

"/*"                           { BEGIN(COMMENT); linhacomment=line;colunacomment=colunas; }
<COMMENT>"*/"                  { BEGIN(INITIAL); }
<COMMENT>\n                    { line += 1; colunas = 1;}
<COMMENT><<EOF>>               { printf("Line %d, column %d: unterminated comment\n", linhacomment, colunacomment); BEGIN(INITIAL); val = 0;} 
<COMMENT>.                     { }

"//"                           { BEGIN(COMMENT2); }
<COMMENT2>\n                   {line += 1; colunas = 1; if(val == 1){ if(TokenOn)printf("SEMICOLON\n"); val = 0; return SEMICOLON;}  BEGIN(INITIAL); }
<COMMENT2>.                    { }

\"                             { BEGIN(STRING); i=0; buffer[0]='\0'; erroescape = 0; linhastring=line; colunastring=colunas-1;val=0; }

<STRING>\n                    {printf("Line %d, column %d: unterminated string literal\n",linhastring, colunastring); 
                                            BEGIN(INITIAL); 
                                            val = 0; 
                                            line++; 
                                            colunas = 1;}
<STRING>\"                    {  
                                  BEGIN(INITIAL);
                                  buffer[i]='\0';
                                  val=0;
                                 snprintf(buffer2, sizeof(buffer2), "\"%s\"", buffer);

                                  if(erroescape != 1){

                                      if(TokenOn)printf("STRLIT(%s)\n",buffer2);
                                      val = 1;
                                      yylval.lexeme = strdup(buffer2);
                                      syn_column=colunastring;
                                      buffer[i]='\0';
                                      return STRLIT;
                                    }
                              }

<STRING>\\["f""n""r""t"\\]     { 
                                      buffer[i++]=yytext[0];
                                      buffer[i++]=yytext[1];
                                      val=0;}

<STRING><<EOF>>               { printf("Line %d, column %d: unterminated string literal\n",linhastring, colunastring); BEGIN(INITIAL);} 

<STRING>\\[\n]                { printf("Line %d, column %d: invalid escape sequence (\\)\n",line,syn_column);
                                  printf("Line %d, column %d: unterminated string literal\n",linhastring, colunastring);
                                  BEGIN(INITIAL);
                                  colunas = 1;
                                  line++;
                                  erroescape = 1;
                                  val=0;}

<STRING>\\[^"f""n""r""t"\\]   { printf("Line %d, column %d: invalid escape sequence (%s)\n",line,syn_column, yytext);
                                  erroescape = 1;
                                  val=0;}

<STRING>.                     { buffer[i++]=yytext[0];
                                  val=0;}

<<EOF>>                       {if(val == 1){if(TokenOn)printf("SEMICOLON\n");  val = 0; return SEMICOLON;};syn_line=line; line++; colunas=1;syn_column=colunas; return 0;}

.                              { printf("Line %d, column %d: illegal character (%s)\n", line, colunas, yytext); }


%%
extern int yylex();
int main(int argc, char **argv) {
    if (argc>1){
        if(strcmp(argv[1],"-l")==0)
            TokenOn = 1;
    }
    //yydebug=1;
    yyparse();
    if(errors==0){
        if (argc>1){
            if(strcmp(argv[1],"-t")==0)
                show(program, 0);
        }
    
        if(argc>1 && strcmp(argv[1],"-s")==0){
            errors += check_program(program);
            show_symbol_table();
            show(program,0);
        }
    }
    /* run the lexical analysis automaton */
    return 0;
}
int yywrap(){
    return 1;
}
void yyerror(char *error) {
    errors++;
    /* yydebug=1; */
    if(strlen(buffer2)> 2 && strcmp(buffer2,"\"\"")!=0){
        printf("Line %d, column %d: %s: %s\n",syn_line,syn_column,error,yylval.lexeme);}
    else{
        printf("Line %d, column %d: %s: %s\n",syn_line,syn_column,error,yytext);
    }

}