#include <bits/stdc++.h>
#include "classes.hpp"

std::vector<int> varid;
std::vector<DecVarE *> globblock;

void Program::codeGen(std::ofstream &out) {
	out << ".data" << std::endl;
	out << ".align 2" << std::endl;
	for (Declaration *d : decs) {
		if (DecVar *dv = dynamic_cast<DecVar *>(d)) {
			out << "var" << dv->identificator << ": .space 4" << std::endl;
			dv->i = -1;
			if (DecVarE *dve = dynamic_cast<DecVarE *>(dv))
				globblock.push_back(dve);
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
			else df->codeGen(out);
		}
	}
	out << "funcprint:" << std::endl;
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
	varid.push_back(0);
	out << (this->identificator == "main" ? "" : "func") << this->identificator << ":" << std::endl;
	out << "\taddiu $fp, $sp, 0" << std::endl;
	if (this->identificator == "main") {
		for (DecVarE *dve : globblock) {
			dve->codeGen(out);
			out << "\tsw $a0, " << "var" + dynamic_cast<DecVar *>(dve)->identificator << std::endl;
		}
	}
	out << "\tsw $ra, 0($sp)" << std::endl;
	out << "\taddiu $sp, $sp, -4" << std::endl;
	if (BlockDS *b = dynamic_cast<BlockDS *>(&(this->block))) {
		for (Declaration *d : b->decs) {
			if (DecVarE *dv = dynamic_cast<DecVarE *>(d)) {
				dv->codeGen(out);
				out << "\tsw $a0, 0($sp)" << std::endl;
				out << "\taddiu $sp, $sp, -4" << std::endl;
				dynamic_cast<DecVar *>(dv)->i = varid.back()++;
			}
			else if (DecVar *dv = dynamic_cast<DecVar *>(d)) {
				dv->codeGen(out);
				dv->i = varid.back()++;
			}
		}
		for (Statement *s : b->statements) {
			if (FuncCall *f = dynamic_cast<FuncCall *>(s)) {
				f->codeGen(out);
			}
			else if (Assignment *f = dynamic_cast<Assignment *>(s)) {
				f->codeGen(out);
			}
		}
	}
	else if (BlockD *b = dynamic_cast<BlockD *>(&(this->block))) {
		for (Declaration *d : b->decs) {
			if (DecVarE *dv = dynamic_cast<DecVarE *>(d)) {
				dv->codeGen(out);
				out << "\tsw $a0, 0($sp)" << std::endl;
				out << "\taddiu $sp, $sp, -4" << std::endl;
				dynamic_cast<DecVar *>(dv)->i = varid.back()++;
			}
			else if (DecVar *dv = dynamic_cast<DecVar *>(d)) {
				dv->codeGen(out);
				dv->i = varid.back()++;
			}
		}
	}
	else if (BlockS *b = dynamic_cast<BlockS *>(&(this->block))) {
		for (Statement *s : b->statements) {
			if (FuncCall *f = dynamic_cast<FuncCall *>(s)) {
				f->codeGen(out);
			}
			else if (Assignment *f = dynamic_cast<Assignment *>(s)) {
				f->codeGen(out);
			}
		}
	}
	out << "\tlw $ra, " << 4 + varid.back() * 4 << "($sp)" << std::endl;
	out << "\taddiu $sp, $sp, " << 8 + varid.back() * 4 << std::endl;
	out << "\tlw $fp, 0($sp)" << std::endl;
	if (this->type.t == 1 && this->identificator != "main") out << "\taddiu $v0, $a0, 0" << std::endl;
	if (this->identificator != "main") {
		out << "\tjr $ra" << std::endl;
		varid.pop_back();
	}
}

void DecVarE::codeGen(std::ofstream &out) {
	if (Integer *i = dynamic_cast<Integer *>(&(this->assignExpression))) {
		i->codeGen(out);
	}
	else if (Var *v = dynamic_cast<Var *>(&(this->assignExpression))) {
		v->codeGen(out);
	}
}

void DecVar::codeGen(std::ofstream &out) {
	out << "\taddiu $sp, $sp, -4" << std::endl;
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
		else if (Integer *i = dynamic_cast<Integer *>(*it)) {
			i->codeGen(out);
			out << "\tsw $a0, 0($sp)" << std::endl;
			out << "\taddiu $sp, $sp, -4" << std::endl;
		}
	}
	out << "\tjal " << "func" << this->identifier << std::endl;
	
}

void Var::codeGen(std::ofstream &out) {
	if (this->dv->i == -1) {
		out << "\tlw $a0," << "var" + name << std::endl;
	}
	else
		out << "\tlw $a0," << -4 * (this->dv->i + 1) << "($fp)" << std::endl;
}

void Assignment::codeGen(std::ofstream &out) {
	if (Var *v = dynamic_cast<Var *>(&(this->value))) {
		v->codeGen(out);
	}
	else if (Integer *i = dynamic_cast<Integer *>(&(this->value))) {
		i->codeGen(out);
	}
	if (this->dv->i == -1)
		out << "\tsw $a0," << "var" + this->identifier << std::endl;
	else
		out << "\tsw $a0," << -4 * (this->dv->i + 1) << "($fp)" << std::endl;
}