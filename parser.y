%{
#include <bits/stdc++.h>
#include "classes.h"

extern int yylex();
extern FILE *yyin;
void yyerror(const char *s) { std::printf("Error: %s\n", s);std::exit(1); }

int errors;
%}

%union {
    std::string *name;
    int token, number;
}

%start program

%token IDENTIFIER
%token SCOL
%token COMMA
%token LPAREN
%token RPAREN
%token LBRACE
%token RBRACE
%token IF
%token ELSE
%token WHILE
%token RETURN
%token CONTINUE
%token NOT
%token PLUS
%token MINUS
%token MULT
%token DIV
%token LESS
%token GRE
%token GEQ
%token LEQ
%token DIFF
%token EQ
%token EQTO
%token DEF
%token BREAK
%token NUM
%token AND
%token OR
%token INTEGER
%token VOID

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
