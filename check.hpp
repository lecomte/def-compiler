#include "classes.hpp"
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
	void checkSym(FuncCall *fc, bool expr);
	void checkSym(IfE *ife, bool f);
	void checkSym(If *i, bool f);
	void checkSym(While *i);
	void checkSym(Var *v);
#endif
