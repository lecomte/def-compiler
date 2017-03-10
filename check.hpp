#include "classes.h"
#include <bits/stdc++.h>
#ifndef CHECK_HPP
#define CHECK_HPP
	extern void yyerror(const char *s);
	extern std::vector<std::vector<Declaration *> *> scopus;
	extern std::vector<DecFunc *> functions;
	extern Program *ast;
	void checkSym();
	void checkSym(DecFunc *df);
	void checkSym(Block *bl, bool f);
	void checkSym(DecVar *dv);
	void checkSym(Statement *s, bool f);
	void checkSym(Expression *e);
	void checkSym(FuncCall *fc);
	void checkSym(IfE *ife);
	void checkSym(If *i);
	void checkSym(While *i);
	void checkSym(Var *v);
#endif
