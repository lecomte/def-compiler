#include <string>
#include <vector>
#include <iostream>
#ifndef CLASSES_H
#define CLASSES_H

class Expression {
	public:
		virtual ~Expression() {}
		std::string print(Expression *a);
		void codeGen(std::ofstream &out);
};

class Statement : public Expression {
	public:
		virtual ~Statement() {}
		std::string print(Statement *a);
		void codeGen(std::ofstream &out);
};

class Declaration {
	public:
		virtual ~Declaration() {}
		std::string print(Declaration *a);
		void codeGen(std::ofstream &out);
};


class Type {
	public:
		int t; /* INT = 1, VOID = 0 */
		Type(int a) : t(a) {}
};

class Operator {
	public:
		std::string op;
		Operator(std::string o) : op(o) {}
	public:
		std::string print() {
			return op;
		}
		void codeGen(std::ofstream &out);
};

class Program {
	public:
		std::vector<Declaration *> decs;
		Program(std::vector<Declaration *> d) : decs(d) {}
	public:
		std::string print() {
			std::string s = "[program";
			for (Declaration *a : decs) {
				s += " ";
				s += a->print(a);
			}
			s += "]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class DecVar : public Declaration {
	public:
		std::string identificator;
		DecVar(std::string i) : identificator(i) {}
	public:
		std::string print() {
			std::string s = "[decvar [" + identificator + "]]";
			return s;
		}
		void codeGen(std::ofstream &out);
};


class Var : public Expression {
	public:
		std::string name;
		DecVar* dv;
		Var(std::string n) : name(n), dv(NULL) {}
		Var(DecVar &d) : name(d.identificator), dv(NULL) {}
	public:
		std::string print() {
			std::string s = "[" + name + "]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class DecVarE : public DecVar {
	public:
		Expression &assignExpression;
		DecVarE(std::string i, Expression &a) : DecVar(i), assignExpression(a) {}
	public:
		std::string print() {
			std::string s = "[decvar [" + DecVar::identificator + "] " + assignExpression.print(&assignExpression) + "]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class Block {
	public:
		virtual ~Block() {}
		std::string print() {
			std::string s = "[block ]";
			return s;
		}
		std::string print(Block *);
		void codeGen(std::ofstream &out);
};

class BlockDS : public Block {
	public:
		std::vector<Declaration *> decs;
		std::vector<Statement *> statements;
		BlockDS(std::vector<Statement *> s, std::vector<Declaration *> d) : statements(s), decs(d) {};
	public:
		std::string print() {
			std::string s = "[block";
			for (Declaration *a : decs) {
				s += " ";
				s += a->print(a);
			}
			for (Statement *a : statements) {
				s += " ";
				s += a->print(a);
			}
			s += "]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class BlockD : public Block {
	public:
		std::vector<Declaration *> decs;
		BlockD(std::vector<Declaration *> d) : decs(d) {};
	public:
		std::string print() {
			std::string s = "[block";
			for (Declaration *a : decs) {
				s += " ";
				s += a->print(a);
			}
			s += "]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class BlockS : public Block {
	public:
		std::vector<Statement *> statements;
		BlockS(std::vector<Statement *> s) : statements(s) {};
	public:
		std::string print() {
			std::string s = "[block";
			for (Statement *a : statements) {
				s += " ";
				s += a->print(a);
			}
			s += "]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class DecFunc : public Declaration {
	public:
		Type type;
		std::string identificator;
		Block &block;
		DecFunc(Type t, std::string i, Block& b) : type(t), identificator(i), block(b) {}
	public:
		std::string print() {
			std::string s = "[decfunc [" + identificator + "] [paramlist] " + block.print(&block);
			
			s += "]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class DecFuncP : public DecFunc {
	public:
		std::vector<Var *> params;
		DecFuncP(Type t, std::string i, std::vector<Var *> p, Block& b) : DecFunc(t, i, b), params(p) {}
	public:
		std::string print() {
			std::string s = "[decfunc [" + identificator + "] [paramlist";
			for (Var *a : params) {
				s += " ";
				s += a->print();
			}
			s += "] " + DecFunc::block.print(&(DecFunc::block));
			
			s += "]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class BinOperation : public Expression {
	public:
		Expression &first, &second;
		std::string op;
		BinOperation(std::string o, Expression &f, Expression &s) : op(o), first(f), second(s) {}
	public:
		std::string print() {
			std::string s = "[" + op + " " + first.print(&first) + " " + second.print(&second);
			s += "]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class UnOperation : public Expression {
	public:
		Expression &exp;
		std::string op;
		UnOperation(std::string o, Expression &e) : exp(e), op(o) {}
	public:
		std::string print() {
			std::string s = "[" + op + " " + exp.print(&exp);
			s += "]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class FuncCall : public Statement {
	public:
		std::string identifier;
		std::vector<Expression *> args;
		FuncCall(std::string i, std::vector<Expression *> a) : identifier(i), args(a) {}
	public:
		std::string print() {
			std::string s = "[funccall [" + identifier + "]" + "[arglist";
			for (Expression *a : args) {
				s += " ";
				s += a->print(a);
			}
			s += "]";
			s += "]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class Integer : public Expression {
	public:
		int number;
		Integer(int n) : number(n) {}
	public:
		std::string print() {
			std::string s = "[" + std::to_string(number);
			s += "]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class Assignment : public Statement {
	public:
		std::string identifier;
		Expression &value;
		Assignment(std::string i, Expression &v) : identifier(i), value(v) {}
	public:
		std::string print() {
			std::string s = "[assign [" + identifier + "] " + value.print(&value);
			s += "]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class Return : public Statement {
	public:
		Expression &value;
		Return(Expression &v) : value(v) {}
	public:
		std::string print() {
			std::string s = "[return " + value.print(&value) + "]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class ReturnV : public Statement {
	public:
		std::string print() {
			std::string s = "[return ]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class IfE : public Statement {
	public:
		Expression &condition;
		Block &yes, &no;
		IfE(Expression &c, Block &y, Block &n) : condition(c), yes(y), no(n) {}
	public:
		std::string print() {
			std::string s = "[if "+ condition.print(&condition) + " " + yes.print(&yes) + " " + no.print(&no) +"]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class If : public Statement {
	public:
		Expression &condition;
		Block &yes;
		If(Expression &c, Block &y) : condition(c), yes(y) {}
	public:
		std::string print() {
			std::string s = "[if "+ condition.print(&condition) + " " + yes.print(&yes) + "]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class While : public Statement {
	public:
		Expression &condition;
		Block &exec;
		While(Expression &c, Block &e) : condition(c), exec(e) {}
	public:
		std::string print() {
			std::string s = "[while "+ condition.print(&condition) + " " + exec.print(&exec) + "]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class Continue : public Statement {
	public:
		std::string print() {
			std::string s = "[continue ]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class Break : public Statement {
	public:
		std::string print() {
			std::string s = "[break ]";
			return s;
		}
		void codeGen(std::ofstream &out);
};

class KeepFunc {
	public:
		Type t;
		std::string id;
		int nParams;
		KeepFunc(DecFunc f) : t(f.type), id(f.identificator), nParams(0) {}
		KeepFunc(DecFuncP f) : t(f.DecFunc::type), id(f.DecFunc::identificator), nParams(f.params.size()) {}
};


#endif
