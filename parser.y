%{
#include <bits/stdc++.h>
#include "classes.h"

extern int yylex();
extern FILE *yyin;
void yyerror(const char *s) { std::printf("Error: %s\n", s);std::exit(1); }

int errors;
Program *ast;
%}

%union {
    std::string *name;
    int token, *number;
    Var *v;
    Program *p;
    Declaration *dc;
    Statement *stm;
    Operator *op;
    Expression *ex;
    std::vector<Declaration *> *dcs;
    std::vector<Var *> *par;
    std::vector<Expression *> *exs;
    std::vector<Statement *> *stms;
}

%start program

%token <name> IDENTIFIER
%token <token> SCOL COMMA LPAREN RPAREN LBRACE RBRACE IF ELSE WHILE RETURN CONTINUE NOT PLUS MINUS MULT DIV LESS GRE GEQ LEQ DIFF EQ EQTO DEF BREAK
%token <number> NUM
%token <token> AND OR INTEGER VOID

%type <p> program;
%type <dc> dec decvar decfunc param
%type <dcs> decs
%type <ex> exp expr
%type <exs> args
%type <par> params
%type <v> param
%type <stm> stmt ifstmt retstmt
%type <stms> stmts

%left PLUS MINUS
%left MULT DIV

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%nonassoc LOWER_THAN_RETURN
%nonassoc RETURN
%%

program : decs
	| /*vazio*/
;

decs : decs dec | dec ;

dec : decvar | decfunc ;

decvar : type IDENTIFIER SCOL
	| type IDENTIFIER EQTO expr
;

decfunc : DEF type IDENTIFIER LPAREN RPAREN block
	| DEF type IDENTIFIER LPAREN params RPAREN block
;

params : params COMMA param
	| param
;

param : type IDENTIFIER;

block : LBRACE decvs stmts RBRACE
	| LBRACE decvs RBRACE
	| LBRACE stmts RBRACE
	| LBRACE RBRACE
;

decvs : decvs decvar | decvar;

stmts : stmts stmt | stmt;

stmt : assign SCOL
	| funccall SCOL
	| ifstmt
	| retstmt
	| BREAK SCOL
	| CONTINUE SCOL
;

ifstmt : IF LPAREN expr RPAREN block %prec LOWER_THAN_ELSE
	| IF LPAREN expr RPAREN block ELSE block
;

retstmt : RETURN SCOL | RETURN expr SCOL
;

assign : IDENTIFIER EQTO expr;

funccall: IDENTIFIER LPAREN args RPAREN | IDENTIFIER LPAREN RPAREN;

args : args COMMA expr | expr;

expr : expr binop exp | exp;

exp : unop exp | LPAREN expr RPAREN | funccall | NUM | IDENTIFIER;

binop : PLUS | MINUS | DIV | MULT | EQ | DIFF | GRE | LESS | GEQ | LEQ | AND | OR;

unop : NOT | MINUS;

type : INTEGER | VOID;

%%

int main( int argc, char *argv[] ) { 
	++argv; --argc;
	yyin = fopen( argv[0], "r" );
	errors = 0;
	yydebug = 1;
	yyparse ();
	return 0;
}
void yyerror (char *s) { /* Called by yyparse on error */
	std::cout << s;
}
