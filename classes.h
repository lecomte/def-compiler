#ifndef CLASSES_H
#define CLASSES_H

class Program {
	public:
		std::vector<Declaration> *decs;
		Program(std::vector<Declaration> *d) : decs(d) {}
};

class Declaration {};

class DecVar : public Declaration {
	public:
		std::string *identificator;
		Expression *assingExpression;
		DecVar(std::string *i, Expression *a) : identificator(i), assignExpression(a) {}
};

class DecFunc : public Declaration {
	public:
		Type *type;
		std::string *identificator;
		std::vector<Var> *params;
		Block *block;
		DecFunc(Type *t, std::string *i, std::vector<Var> *p, Block *b) : type(t), identificator(i), params(p), block(b) {}
};

class Expression {};

class BinOperation : public Expression {
	public:
		Expression *first, *second;
		Operator *op;
		BinOperation(Operator *o, Expression *f, Expression *s) : op(o), first(f), second(s) {}
};

#endif
