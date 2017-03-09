%{
#include <bits/stdc++.h>
#include <vector>
#include "classes.h"

extern int yylex();
extern FILE *yyin;
void yyerror(const char *s) { std::printf("Error: %s\n", s);std::exit(1); }

int errors;
Program *ast;


	std::string Expression::print(Expression *a) {
			if (Var* t = dynamic_cast<Var*>(a)) {
        		return t->print();
    		}
    		else if (FuncCall* t = dynamic_cast<FuncCall*>(a)) {
        		return t->print();
    		}
    		else if (UnOperation* t = dynamic_cast<UnOperation*>(a)) {
        		return t->print();
    		}
    		else if (BinOperation* t = dynamic_cast<BinOperation*>(a)) {
        		return t->print();
    		}
    		else if (Integer* t = dynamic_cast<Integer*>(a)) {
        		return t->print();
    		}
    		return "";
		}
		
		std::string Statement::print(Statement *a) {
			if (IfE* t = dynamic_cast<IfE*>(a)) {
        		return t->print();
    		}
    		else if (If* t = dynamic_cast<If*>(a)) {
        		return t->print();
    		}
    		else if (While* t = dynamic_cast<While*>(a)) {
        		return t->print();
    		}
    		else if (Return* t = dynamic_cast<Return*>(a)) {
        		return t->print();
    		}
    		else if (ReturnV* t = dynamic_cast<ReturnV*>(a)) {
        		return t->print();
    		}
    		else if (BlockDS* t = dynamic_cast<BlockDS*>(a)) {
        		return t->print();
    		}
    		else if (BlockD* t = dynamic_cast<BlockD*>(a)) {
        		return t->print();
    		}
    		else if (BlockS* t = dynamic_cast<BlockS*>(a)) {
        		return t->print();
    		}
    		else if (Block* t = dynamic_cast<Block*>(a)) {
        		return t->print();
    		}
    		else if (Continue* t = dynamic_cast<Continue*>(a)) {
        		return t->print();
    		}
    		else if (Break* t = dynamic_cast<Break*>(a)) {
        		return t->print();
    		}
    		else if (Assignment* t = dynamic_cast<Assignment*>(a)) {
        		return t->print();
    		}
    		else if (FuncCall* t = dynamic_cast<FuncCall*>(a)) {
        		return t->print();
    		}
    		return "";
		}
		
		std::string Declaration::print(Declaration *a) {
			if (DecVarE* t = dynamic_cast<DecVarE*>(a)) {
        		return t->print();
    		}
			else if (DecVar* t = dynamic_cast<DecVar*>(a)) {
        		return t->print();
    		}
    		else if (DecFuncP* t = dynamic_cast<DecFuncP*>(a)) {
        		return t->print();
    		}
    		else if (DecFunc* t = dynamic_cast<DecFunc*>(a)) {
        		return t->print();
    		}
    		return "";
		}
		std::string Block::print(Block * a) {
			if (BlockDS* t = dynamic_cast<BlockDS*>(a)) {
        		return t->print();
    		}
    		else if (BlockD* t = dynamic_cast<BlockD*>(a)) {
        		return t->print();
    		}
    		else if (BlockS* t = dynamic_cast<BlockS*>(a)) {
        		return t->print();
    		}
    		else {
        		return t->print();
    		}
		}
%}

%union {
    std::string *name;
    int token, number;
    Block *b;
    Var *v;
    Program *p;
    Declaration *dc;
    Statement *stm;
    Type *t;
    Operator *op;
    Expression *ex;
    std::vector<Declaration *> *dcs;
    std::vector<Var *> *par;
    std::vector<Expression *> *exs;
    std::vector<Statement *> *stms;
}

%start program

%token <name> IDENTIFIER NOT PLUS MINUS MULT DIV LESS GRE GEQ LEQ DIFF EQ AND OR
%token <token> SCOL COMMA LPAREN RPAREN LBRACE RBRACE IF ELSE WHILE RETURN CONTINUE EQTO DEF BREAK
%token <number> NUM
%token <token> INTEGER VOID

%type <p> program;
%type <dc> dec decvar decfunc
%type <dcs> decs decvs
%type <ex> exp expr funccall
%type <exs> args
%type <par> params
%type <v> param
%type <stm> stmt ifstmt retstmt assign
%type <stms> stmts
%type <b> block
%type <t> type
%type <name> binop unop

%left PLUS MINUS
%left MULT DIV

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%nonassoc LOWER_THAN_RETURN
%nonassoc RETURN
%%

program : decs {$$ = new Program(*$1); ast = $$;}
	| /*vazio*/ {$$ = NULL;}
;

decs : decs dec {$1->push_back($2);} | dec {$$ = new std::vector<Declaration *>; $$->push_back($1);} ;

dec : decvar | decfunc ;

decvar : type IDENTIFIER SCOL {$$ = (Declaration *) new DecVar(*$2);}
	| type IDENTIFIER EQTO expr {$$ = (Declaration *) new DecVarE(*$2, *$4);}
;

decfunc : DEF type IDENTIFIER LPAREN RPAREN block {$$ = (Declaration *) new DecFunc(*$2, *$3, *$6);}
	| DEF type IDENTIFIER LPAREN params RPAREN block {$$ = (Declaration *) new DecFuncP(*$2, *$3, *$5, *$7);}
;

params : params COMMA param {$1->push_back($3); $$ = $1;}
	| param {$$ = new std::vector<Var *>; $$->push_back($1);}
;

param : type IDENTIFIER {$$ = new Var(*$2);};

block : LBRACE decvs stmts RBRACE {$$ = new BlockDS(*$3, *$2);}
	| LBRACE decvs RBRACE {$$ = new BlockD(*$2);}
	| LBRACE stmts RBRACE {$$ = new BlockS(*$2);}
	| LBRACE RBRACE {$$ = new Block();}
;

decvs : decvs decvar {$1->push_back($2); } | decvar {$$ = new std::vector<Declaration *>; $$->push_back($1);};

stmts : stmts stmt {$1->push_back($2); } | stmt {$$ = new std::vector<Statement *>; $$->push_back($1);};

stmt : assign SCOL
	| funccall SCOL {$$ = (Statement *) $1;}
	| ifstmt
	| WHILE LPAREN expr RPAREN block {$$ = (Statement *) new While(*$3, *$5);}
	| retstmt
	| BREAK SCOL {$$ = (Statement *) new Break;}
	| CONTINUE SCOL {$$ = (Statement *) new Continue;}
;

ifstmt : IF LPAREN expr RPAREN block %prec LOWER_THAN_ELSE {$$ = (Statement *) new If(*$3, *$5);}
	| IF LPAREN expr RPAREN block ELSE block {$$ = (Statement *) new IfE(*$3, *$5, *$7);}
;

retstmt : RETURN SCOL {$$ = (Statement *) new ReturnV();} | RETURN expr SCOL {$$ = (Statement *) new Return(*$2);}
;

assign : IDENTIFIER EQTO expr {$$ = (Statement *) new Assignment(*$1, *$3);};

funccall: IDENTIFIER LPAREN args RPAREN {$$ = (Expression *) new FuncCall(*$1, *$3);} | IDENTIFIER LPAREN RPAREN {std::vector<Expression *> s; $$ = (Expression *) new FuncCall(*$1, s);}; 

args : args COMMA expr {$1->push_back($3); $$ = $1;} | expr {$$ = new std::vector<Expression *>; $$->push_back($1);};

expr : expr binop exp {$$ = (Expression *) new BinOperation(*$2, *$1, *$3); } | exp;

exp : unop exp {$$ = (Expression *) new UnOperation(*$1, *$2);} | LPAREN expr RPAREN {$$ = $2;} | funccall | NUM {$$ = (Expression *) new Integer($1);} | IDENTIFIER {$$ = (Expression *) new Var(*$1);};

binop : PLUS | MINUS | DIV | MULT | EQ | DIFF | GRE | LESS | GEQ | LEQ | AND | OR ;

unop : NOT | MINUS 

type : INTEGER {$$ = new Type(1);} | VOID {$$ = new Type(0);};

%%

int main( int argc, char *argv[] ) { 
	++argv; --argc;
	yyin = fopen( argv[0], "r" );
	errors = 0;
	yyparse ();
	std::cout << ast->print() << std::endl;
	return 0;
}
void yyerror (char *s) { /* Called by yyparse on error */
	std::cout << s;
}
