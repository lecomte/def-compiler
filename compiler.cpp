#include <bits/stdc++.h>
#include "classes.hpp"
#include "check.hpp"
#include "parser.hpp"

extern FILE *yyin;
extern int errors;

int main( int argc, char *argv[] ) { 
	std::vector<Var *> p;
	p.push_back(new Var("output"));
	functions.push_back(new DecFuncP(*(new Type(1)), "print", p , *(new Block())));
	++argv; --argc;
	yyin = fopen( argv[0], "r" );
	std::ofstream out;
	out.open(argv[1]);
	errors = 0;
	yyparse ();
	checkSym();
	DecFunc *ma = NULL;
	for (DecFunc *f : functions) {
		if (f->identificator == "main")
			ma = f;
	}
	if (ma == NULL) {
		yyerror("Program lacks main function");
		return 1;
	}
	out << ast->print() << std::endl;
	return 0;
}
