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
    Block *b;
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
%type <dcs> decs decvs
%type <ex> exp expr funccall
%type <exs> args
%type <par> params
%type <v> param
%type <stm> stmt ifstmt retstmt assign
%type <stms> stmts
%type <b> block

%left PLUS MINUS
%left MULT DIV

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%nonassoc LOWER_THAN_RETURN
%nonassoc RETURN
%%

program : decs {$$ = new Program($1);}
	| /*vazio*/ {$$ = NULL;}
;

decs : decs dec {$1->push_back($2); $$ = $1;} | dec {$$ = new std::vector<Declaration *>; $$->push_back($1);} ;

dec : decvar | decfunc ;

decvar : type IDENTIFIER SCOL {$$ = (Declaration *) new DecVar($2, NULL);}
	| type IDENTIFIER EQTO expr {$$ = (Declaration *) new DecVar($2, $4);}
;

decfunc : DEF type IDENTIFIER LPAREN RPAREN block {$$ = (Declaration *) new DecFunc($2, $3, NULL, $6);}
	| DEF type IDENTIFIER LPAREN params RPAREN block {$$ = (Declaration *) new DecFunc($2, $3, $5, $7);}
;

params : params COMMA param {$1->push_back($3); $$ = $1;}
	| param {$$ = new std::vector<Var *>; $$->push_back($1);}
;

param : type IDENTIFIER {$$ = new Var($2);};

block : LBRACE decvs stmts RBRACE {$$ = new Block($3, $2);}
	| LBRACE decvs RBRACE {$$ = new Block(NULL, $2);}
	| LBRACE stmts RBRACE {$$ = new Block($2, NULL);}
	| LBRACE RBRACE {$$ = new Block(NULL, NULL);}
;

decvs : decvs decvar {$1->push_back($2); $$ = $1;} | decvar {$$ = new std::vector<Declaration *>; $$->push_back($1);};

stmts : stmts stmt {$1->push_back($2); $$ = $1;} | stmt {$$ = new std::vector<Statement *>; $$->push_back($1);};

stmt : assign SCOL
	| funccall SCOL {$$ = (Statement *) $1;}
	| ifstmt
	| WHILE LPAREN expr RPAREN block {$$ = (Statement *) new While($3, $5);}
	| retstmt
	| BREAK SCOL {$$ = (Statement *) new Break;}
	| CONTINUE SCOL {$$ = (Statement *) new Continue;}
;

ifstmt : IF LPAREN expr RPAREN block %prec LOWER_THAN_ELSE {$$ = (Statement *) new If($3, $5, NULL);}
	| IF LPAREN expr RPAREN block ELSE block {$$ = (Statement *) new If($3, $5, $7);}
;

retstmt : RETURN SCOL {$$ = (Statement *) new Return(NULL);} | RETURN expr SCOL {$$ = (Statement *) new Return($2);}
;

assign : IDENTIFIER EQTO expr {$$ = (Statement *) new Assignment($1, $3);};

funccall: IDENTIFIER LPAREN args RPAREN {$$ = (Expression *) new FuncCall($1, $2);} | IDENTIFIER LPAREN RPAREN {$$ = (Expression *) new FuncCall($1, NULL);}; 

args : args COMMA expr {$1->push_back($3); $$ = $1;} | expr {$$ = new std::vector<Expression *>; $$->push_back($1);};

expr : expr binop exp {$$ = (Expression *) new BinOperation($2, $1, $3);} | exp;

exp : unop exp {$$ = (Expression *) new UnOperation($1, $2);} | LPAREN expr RPAREN {$$ = $2;} | funccall | NUM {$$ = (Expression *) new Integer($1);} | IDENTIFIER {$$ = (Expression *) new Var($1);};

binop : PLUS {$$ = new Operator(new std::string("+"));} | MINUS {$$ = new Operator(new std::string("-"));} | DIV {$$ = new Operator(new std::string("/"));} | MULT {$$ = new Operator(new std::string("*"));} | EQ {$$ = new Operator(new std::string("=="));} | DIFF {$$ = new Operator(new std::string("!="));} | GRE {$$ = new Operator(new std::string(">"));} | LESS {$$ = new Operator(new std::string("<"));} | GEQ {$$ = new Operator(new std::string(">="));} | LEQ {$$ = new Operator(new std::string("<="));} | AND {$$ = new Operator(new std::string("&&"));} | OR {$$ = new Operator(new std::string("||"));};

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
