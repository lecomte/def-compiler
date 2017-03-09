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

class UnOperation : public Expression {
	public:
		Expression *exp;
		Operator *op;
		UnOperation(Expression *e, Operator *o) : exp(e), op(o) {}
};

class Statement : public Expression {};

class FuncCall : public Statement {
	public:
		std::string *identifier;
		std::vector<Expression> *args;
		FuncCall(std::string *i, std::vector<Expression> *a) : identifier(i), args(a) {}
};

class Integer : public Expression {
	public:
		int *number;
		Integer(int *n) : number(n) {}
}

#endif
