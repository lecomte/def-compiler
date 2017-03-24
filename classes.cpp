#include <bits/stdc++.h>
#include "classes.hpp"

void Program::codeGen(std::ofstream &out) {
	out << ".data" << std::endl;
	out << ".align 2" << std::endl;
	for (Declaration *d : decs) {
		if (DecVar *dv = dynamic_cast<DecVar *>(d)) {
			out << "var" << dv->identificator << ": .space 4" << std::endl;
		}
	}
	out << ".text" << std::endl;
	out << ".globl main" << std::endl;
	for (Declaration *d : decs) {
		if (DecFunc *df = dynamic_cast<DecFunc *>(d)) {
			if (df->identificator == "main") {
				df->codeGen(out);
				out << "\t" << "li $v0, 10" << std::endl;
				out << "\t" << "syscall" << std::endl;
				break;
			}
		}
	}
	out << "print:" << std::endl;
	out << "\taddiu $fp, $sp, 0" << std::endl;
	out << "\tsw $ra, 0($sp)" << std::endl;
	out << "\taddiu $sp, $sp, -4" << std::endl;
	out << "\tlw $a0, 4($fp)" << std::endl;
	out << "\tli $v0, 1" << std::endl;
	out << "\tsyscall" << std::endl;
	out << "\tli $a0, 10" << std::endl;
	out << "\tli $v0, 11" << std::endl;
	out << "\tsyscall" << std::endl;
	out << "\tlw $ra, 4($sp)" << std::endl;
	out << "\taddiu $sp, $sp, 12" << std::endl;
	out << "\tlw $fp, 0($sp)" << std::endl;
	out << "\tjr $ra" << std::endl;
}

void DecFunc::codeGen(std::ofstream &out) {
	out << this->identificator << ":" << std::endl;
	if (this->identificator == "main") out << "move $fp, $sp" << std::endl;
	if (BlockDS *b = dynamic_cast<BlockDS *>(&(this->block))) {
		int i = 0;
		for (Declaration *d : b->decs) {
			if (DecVarE *dv = dynamic_cast<DecVarE *>(d)) {
				dv->codeGen(out);
				out << "\tsw $a0, 0($sp)" << std::endl;
				out << "\taddiu $sp, $sp, -4" << std::endl;
				dynamic_cast<DecVar *>(dv)->i = i++;
			}
		}
		for (Statement *s : b->statements) {
			if (FuncCall *f = dynamic_cast<FuncCall *>(s)) {
				f->codeGen(out);
			}
		}
	}
}

void DecVarE::codeGen(std::ofstream &out) {
	if (Integer *i = dynamic_cast<Integer *>(&(this->assignExpression))) {
		i->codeGen(out);
	}
}

void Integer::codeGen(std::ofstream &out) {
	out << "\tli $a0, " << this->number << std::endl;
}

void FuncCall::codeGen(std::ofstream &out) {
	out << "\tsw $fp, 0($sp)" << std::endl;
	out << "\taddiu $sp, $sp, -4" << std::endl;
	for (std::vector<Expression *>::reverse_iterator it = this->args.rbegin(); it != args.rend(); it++) {
		if (Var *v = dynamic_cast<Var *>(*it)) {
			v->codeGen(out);
			out << "\tsw $a0, 0($sp)" << std::endl;
			out << "\taddiu $sp, $sp, -4" << std::endl;
		}
	}
	out << "\tjal " << this->identifier << std::endl;
	
}

void Var::codeGen(std::ofstream &out) {
	out << "\tlw $a0," << -4 * this->dv->i << "($fp)" << std::endl;
}
