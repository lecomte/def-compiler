#include <bits/stdc++.h>
#include "classes.hpp"	
#include "check.hpp"	

bool ftype;
	
	void checkSym() {
		if (ast != NULL) {
			scopus.push_back(new std::vector<Declaration *>);
			for (Declaration *d : ast->decs) {
				if (DecVar* t = dynamic_cast<DecVar*>(d)) {
        			checkSym(t);
    			}
    			else if (DecFuncP* t = dynamic_cast<DecFuncP*>(d)) {
        			checkSym(t);
    			}
    			else if (DecFunc* t = dynamic_cast<DecFunc*>(d)) {
        			checkSym(t);
    			}	
			}
		}	
	}
	void checkSym(DecFunc *df) {
		for (DecFunc * v : functions) {
        	if (v->identificator == df->identificator) {
        		yyerror("Variable was already declared");
        		return;
        	}
        }
        functions.push_back(df);
        ftype = (df->type.t == 1);
        scopus.push_back(new std::vector<Declaration *>);
        if (DecFuncP *dfp = dynamic_cast<DecFuncP *>(df)) {
			for (Var *v : dfp->params) {
				checkSym(new DecVar(v->name));
			}
		}
		checkSym(&(df->block), false);
		scopus.pop_back();
	}
	
	void checkSym(Block *bl, bool f) {
		scopus.push_back(new std::vector<Declaration *>);
		if (BlockDS *b = dynamic_cast<BlockDS *>(bl)) {
			for (Declaration *d : b->decs) {
				checkSym(dynamic_cast<DecVar *>(d));
			}
			for (Statement *s : b->statements) {
				checkSym(s, f);
			}
		}
		else if (BlockD *b = dynamic_cast<BlockD *>(bl)) {
			for (Declaration *d : b->decs) {
				checkSym(dynamic_cast<DecVar *>(d));
			}
		}
		else if (BlockS *b = dynamic_cast<BlockS *>(bl)) {
			for (Statement *s : b->statements) {
				checkSym(s, f);
			}
		}
		scopus.pop_back();
	}
	
	void checkSym(DecVar *dv) {
		for (Declaration *d : *(scopus.back())) {
			if (dynamic_cast<DecVar *>(d)->identificator == dv->identificator) {
				yyerror("Variable was already declared");
				return;
			}
		}
		scopus.back()->push_back(dv);
		if (DecVarE *dve = dynamic_cast<DecVarE *>(dv)) {
			checkSym(&(dve->assignExpression));
		}
	}
	void checkSym(Statement *s, bool f) {
		if (Assignment *ass = dynamic_cast<Assignment *>(s)) {
			checkSym(&(ass->value));
		}
		else if (FuncCall *fc = dynamic_cast<FuncCall *>(s)) {
			checkSym(fc, false);
		}
		else if (IfE *ifs = dynamic_cast<IfE *>(s)) {
			checkSym(ifs, f);
		}
		else if (If *ifs = dynamic_cast<If *>(s)) {
			checkSym(ifs, f);
		}
		else if (While *wh = dynamic_cast<While *>(s)) {
			checkSym(wh);
		}
		else if (Break *b = dynamic_cast<Break *>(s)) {
			if (!f) yyerror("Cannot break outside of a loop");
		}
		else if (Continue *b = dynamic_cast<Continue *>(s)) {
			if (!f) yyerror("Cannot continue outside of a loop");
		}
		else if (Return *b = dynamic_cast<Return *>(s)) {
			if (!ftype) yyerror("Function of type void returning int");
			checkSym(&(b->value));
		}
		else if (ReturnV *b = dynamic_cast<ReturnV *>(s)) {
			if (ftype) yyerror("Function of type int returning void");
		}
	}
	
	void checkSym(Expression *e) {
		if (Var *v = dynamic_cast<Var *>(e)) {
			checkSym(v);
		}
		else if (BinOperation *b = dynamic_cast<BinOperation *>(e)) {
			checkSym(&(b->first));
			checkSym(&(b->second));
		}
		else if (UnOperation *b = dynamic_cast<UnOperation *>(e)) {
			checkSym(&(b->exp));
		}
		else if (FuncCall *b = dynamic_cast<FuncCall *>(e)) {
			checkSym(b, true);
		}
	}
	
	void checkSym(FuncCall *fc, bool expr) {
		DecFunc *d = NULL;
		for (DecFunc *df : functions) {
			if (df->identificator == fc->identifier) {
				d = df;
			}
		}
		if (d == NULL) {
			yyerror("Function not declared");
			return;
		}
		else if (expr && d->type.t == 0) yyerror("Expression value cannot be of type void");
		DecFuncP *dfp = dynamic_cast<DecFuncP *>(d);
		if (dfp == NULL && fc->args.size() != 0) {
			yyerror("Too many arguments in function call");
			return;
		}
		if (dfp != NULL && dfp->params.size() != fc->args.size()) {
			yyerror("Too many arguments in function call");
			return;
		}
		for (Expression *e : fc->args) {
			checkSym(e);
		}
	}
	
	void checkSym(IfE *ife, bool f) {
		checkSym(&(ife->condition));
		checkSym(&(ife->yes),f);
		checkSym(&(ife->no),f);
	}
	
	void checkSym(If *i, bool f) {
		checkSym(&(i->condition));
		checkSym(&(i->yes),f);
	}
	
	void checkSym(While *i) {
		checkSym(&(i->condition));
		checkSym(&(i->exec),true);
	}
	
	void checkSym(Var *v) {
		Declaration *de = NULL;
		for (std::vector<std::vector<Declaration *> *>::reverse_iterator it = scopus.rbegin(); it != scopus.rend(); it++) {
			for (Declaration *d : *(*it)) {
				if (v->name == dynamic_cast<DecVar *>(d)->identificator) {
					de = d;
				}
			}
		}
		if (de == NULL) {
			yyerror("Variable not declared");
			return;
		}
	}
