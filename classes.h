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

#endif
