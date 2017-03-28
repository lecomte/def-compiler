#include <bits/stdc++.h>
#include "classes.hpp"

std::vector<int> varid;
std::vector<DecVarE *> globblock;
std::vector<std::string> genBlock;
std::vector<std::string> genWh;
int ifid = 0, whileid = 0, sincefunc;
std::string currfunc;

void Program::codeGen(std::ostream &out) {
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
				break;
			}
			else df->codeGen(out);
		}
	}
	for (std::string s : genBlock) {
		out << s;
	}
	out << std::endl;
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
	out << std::endl;
	out << "true:" << std::endl;
	out << "\tli $a0, 1" << std::endl;
	out << "\tjr $ra" << std::endl;
	out << std::endl;
	out << "false:" << std::endl;
	out << "\tli $a0, 0" << std::endl;
	out << "\tjr $ra" << std::endl;
	out << std::endl;
	out << "greater:" << std::endl;
	out << "\tbgt $t1, $a0, true" << std::endl;
	out << "\tble $t1, $a0, false" << std::endl;
	out << std::endl;
	out << "greatereq:" << std::endl;
	out << "\tbge $t1, $a0, true" << std::endl;
	out << "\tblt $t1, $a0, false" << std::endl;
	out << std::endl;
	out << "less:" << std::endl;
	out << "\tblt $t1, $a0, true" << std::endl;
	out << "\tbge $t1, $a0, false" << std::endl;
	out << std::endl;
	out << "lesseq:" << std::endl;
	out << "\tble $t1, $a0, true" << std::endl;
	out << "\tbgt $t1, $a0, false" << std::endl;
	out << std::endl;
	out << "equal:" << std::endl;
	out << "\tbeq $t1, $a0, true" << std::endl;
	out << "\tbne $t1, $a0, false" << std::endl;
	out << std::endl;
	out << "diff:" << std::endl;
	out << "\tbne $t1, $a0, true" << std::endl;
	out << "\tbeq $t1, $a0, false" << std::endl;
	out << std::endl;
	out << "land:" << std::endl;
	out << "\tbeq $t1, $zero, false" << std::endl;
	out << "\tbeq $a0, $zero, false" << std::endl;
	out << "\tj true" << std::endl;
	out << std::endl;
	out << "lor:" << std::endl;
	out << "\tbne $t1, $zero, true" << std::endl;
	out << "\tbne $a0, $zero, true" << std::endl;
	out << "\tj false" << std::endl;
	out << std::endl;
	out << "lnot:" << std::endl;
	out << "\tbne $a0, $zero, false" << std::endl;
	out << "\tbeq $a0, $zero, true" << std::endl;
}

void DecFunc::codeGen(std::ostream &out) {
	varid.push_back(0);
	std::stringstream cabou;
	cabou << std::endl;
	out << std::endl;
	out << (this->identificator == "main" ? "" : "func") << this->identificator << ":" << std::endl;
	
	cabou << "end_" << (this->identificator == "main" ? "" : "func") << this->identificator << ":" << std::endl;
	
	currfunc = (this->identificator == "main" ? "" : "func") + this->identificator;
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
			s->codeGen(out);
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
			if (If *i = dynamic_cast<If *>(s)) {
				sincefunc = varid.back();
			}
			else if (IfE *i = dynamic_cast<IfE *>(s)) {
				sincefunc = varid.back();
			}
			s->codeGen(out);
		}
	}
	cabou << "\tlw $ra, " << 4 + varid.back() * 4 << "($sp)" << std::endl;
	if (DecFuncP *dfp = dynamic_cast<DecFuncP *>(this)) {
		cabou << "\taddiu $sp, $sp, " << 8 + (varid.back() + dfp->params.size()) * 4 << std::endl;
	}
	else
		cabou << "\taddiu $sp, $sp, " << 8 + varid.back() * 4 << std::endl;
	cabou << "\tlw $fp, 0($sp)" << std::endl;
	if (this->type.t == 1 && this->identificator != "main") cabou << "\taddiu $v0, $a0, 0" << std::endl;
	if (this->identificator != "main") {
		cabou << "\tjr $ra" << std::endl;
		varid.pop_back();
	}
	out << "\tj " << "end_"  << (this->identificator == "main" ? "" : "func") << this->identificator << std::endl;
	if (this->identificator == "main") {
		cabou << "\t" << "li $v0, 10" << std::endl;
		cabou << "\t" << "syscall" << std::endl;
	} 
	genBlock.push_back(cabou.str());
}

void DecVarE::codeGen(std::ostream &out) {
	this->assignExpression.codeGen(out);
}

void DecVar::codeGen(std::ostream &out) {
	out << "\taddiu $sp, $sp, -4" << std::endl;
}

void Integer::codeGen(std::ostream &out) {
	out << "\tli $a0, " << this->number << std::endl;
}

void Statement::codeGen(std::ostream &out) {
	if (FuncCall *f = dynamic_cast<FuncCall *>(this)) {
		f->codeGen(out);
	}
	else if (Assignment *f = dynamic_cast<Assignment *>(this)) {
		f->codeGen(out);
	}
	else if (If *f = dynamic_cast<If *>(this)) {
		f->id = ifid++;
		f->codeGen(out);
	}
	else if (Return *r = dynamic_cast<Return *>(this)) {
		r->codeGen(out);
	}
}

void If::codeGen(std::ostream &out) {
	varid.back()++;
	
	std::ostringstream ass, end;
	ass << std::endl;
	ass << ("if_" + std::to_string(this->id) + ":\n");
	ass << "\tsw $ra, 0($sp)" << std::endl;
	ass << "\taddiu $sp, $sp, -4" << std::endl;
	if (BlockDS *b = dynamic_cast<BlockDS *>(&this->yes)) {
		ass << "\taddiu $sp, $sp," << -4 * b->decs.size() << std::endl;
	}
	else if (BlockD *b = dynamic_cast<BlockD *>(&this->yes)) {
		ass << "\taddiu $sp, $sp," << -4 * b->decs.size() << std::endl;
	}
	out << "\tjal " << "if_" << this->id << std::endl;
	
	this->condition.codeGen(ass);
	ass << "\tbeq $a0, $zero, endif_" << this->id << std::endl;
	if (BlockDS *b = dynamic_cast<BlockDS *>(&this->yes)) {
		for (Declaration *d : b->decs) {
			if (DecVar *dv = dynamic_cast<DecVar *>(d)) {
				dv->i = varid.back()++;
				if (DecVarE *dve = dynamic_cast<DecVarE *>(dv)) {
					dve->codeGen(ass);
					ass << "\tsw $a0," + std::to_string(-4 * (dv->i + 1)) + "($fp)\n";
				}
			}
		}
		for (Statement *s : b->statements) {
			if (Return *r = dynamic_cast<Return *>(s)) {
				r->value.codeGen(ass);
				ass << "\tmove $v0, $a0" << std::endl;
				ass << "\tlw $ra, 0($fp)" << std::endl;
				if (BlockDS *b = dynamic_cast<BlockDS *>(&this->yes)) {
					ass << "\taddiu $sp, $sp," << 4 * (b->decs.size() + 1) << std::endl;
				}
				else if (BlockD *b = dynamic_cast<BlockD *>(&this->yes)) {
					ass << "\taddiu $sp, $sp," << 4 * (b->decs.size() + 1) << std::endl;
				}
				else {
					ass << "\taddiu $sp, $sp, 4" << std::endl;
				}
				ass << "\tj " << "end_"  << currfunc << std::endl;
			}
			else s->codeGen(ass);
		}
	}
	else if (BlockD *b = dynamic_cast<BlockD *>(&this->yes)) {
		for (Declaration *d : b->decs) {
			if (DecVar *dv = dynamic_cast<DecVar *>(d)) {
				dv->i = varid.back()++;
				if (DecVarE *dve = dynamic_cast<DecVarE *>(dv)) {
					dve->codeGen(ass);
					ass << "\tsw $a0," + std::to_string(-4 * (dv->i + 1)) + "($fp)\n";
				}
			}
		}
	}
	else if (BlockS *b = dynamic_cast<BlockS *>(&this->yes)) {
		for (Statement *s : b->statements) {
			if (Return *r = dynamic_cast<Return *>(s)) {
				r->value.codeGen(ass);
				ass << "\tmove $v0, $a0" << std::endl;
				ass << "\tlw $ra, 0($fp)" << std::endl;
				if (BlockDS *b = dynamic_cast<BlockDS *>(&this->yes)) {
					ass << "\taddiu $sp, $sp," << 4 * (b->decs.size() + 1) << std::endl;
				}
				else if (BlockD *b = dynamic_cast<BlockD *>(&this->yes)) {
					ass << "\taddiu $sp, $sp," << 4 * (b->decs.size() + 1) << std::endl;
				}
				else {
					ass << "\taddiu $sp, $sp, 4" << std::endl;
				}
				ass << "\tj " << "end_"  << currfunc << std::endl;
			}
			else s->codeGen(ass);
		}
	}
	ass << "\tj endif_" << this->id << std::endl;
	genBlock.push_back(ass.str());
	end << std::endl;
	end << "endif_" << this->id << ":" << std::endl;
	if (BlockDS *b = dynamic_cast<BlockDS *>(&this->yes)) {
		end << "\taddiu $sp, $sp," << 4 * (b->decs.size() + 1) << std::endl;
		varid.back() -= b->decs.size() + 1;
	}
	else if (BlockD *b = dynamic_cast<BlockD *>(&this->yes)) {
		end << "\taddiu $sp, $sp," << 4 * (b->decs.size() + 1) << std::endl;
		varid.back() -= b->decs.size() + 1;
	}
	else if (BlockS *b = dynamic_cast<BlockS *>(&this->yes)) {
		end << "\taddiu $sp, $sp, 4" << std::endl;
		varid.back() -= 1;
	}
	end << "\tlw $ra, 0($sp)" << std::endl;
	end << "\tjr $ra" << std::endl;
	
	genBlock.push_back(end.str());
}

void Statement::codeGen(std::ostream &out, bool a) {
	if (FuncCall *f = dynamic_cast<FuncCall *>(this)) {
		f->codeGen(out);
	}
	else if (Assignment *f = dynamic_cast<Assignment *>(this)) {
		f->codeGen(out);
	}
}

void Expression::codeGen(std::ostream &out) {
	if (Var *v = dynamic_cast<Var *>(this)) {
		v->codeGen(out);
	}
	else if (Integer *i = dynamic_cast<Integer *>(this)) {
		i->codeGen(out);
	}
	else if (BinOperation *b = dynamic_cast<BinOperation *>(this)) {
		b->codeGen(out);
	}
	else if (UnOperation *u = dynamic_cast<UnOperation *>(this)) {
		u->codeGen(out);
	}
	else if (FuncCall *f = dynamic_cast<FuncCall *>(this)) {
		f->codeGen(out);
		out << "\tmove $a0, $v0" << std::endl;
	}
}

void UnOperation::codeGen(std::ostream &out) {
	this->exp.codeGen(out);
	if (op == "!") {
		out << "\tjal lnot" << std::endl;
	}
	else if (op == "-") {
		out << "\tli $t1, -1" << std::endl;
		out << "\tmult $a0, $t1" << std::endl;
		out << "\tmflo $a0" << std::endl;
	}
}

void BinOperation::codeGen(std::ostream &out) {
	this->first.codeGen(out);
	out << "\tsw $a0, 0($sp)" << std::endl;
	out << "\taddiu $sp, $sp, -4" << std::endl;
	this->second.codeGen(out);
	out << "\tlw $t1, 4($sp)" << std::endl;
	if (op == "+") {
		out << "\tadd $a0, $a0, $t1" << std::endl;
	}
	else if (op == "-") {
		out << "\tsub $a0, $t1, $a0" << std::endl;
	}
	else if (op == "*") {
		out << "\tmult $a0, $t1" << std::endl;
		out << "\tmflo $a0" << std::endl;
	}
	else if (op == "/") {
		out << "\tdiv $t1, $a0" << std::endl;
		out << "\tmflo $a0" << std::endl;
	}
	else if (op == ">") {
		out << "\tjal greater" << std::endl;
	}
	else if (op == ">=") {
		out << "\tjal greatereq" << std::endl;
	}
	else if (op == "<") {
		out << "\tjal less" << std::endl;
	}
	else if (op == "<=") {
		out << "\tjal lesseq" << std::endl;
	}
	else if (op == "==") {
		out << "\tjal equal" << std::endl;
	}
	else if (op == "!=") {
		out << "\tjal diff" << std::endl;
	}
	else if (op == "&&") {
		out << "\tjal land" << std::endl;
	}
	else if (op == "||") {
		out << "\tjal lor" << std::endl;
	}
	out << "\taddiu $sp, $sp, 4" << std::endl;
}

void FuncCall::codeGen(std::ostream &out) {
	out << "\tsw $fp, 0($sp)" << std::endl;
	out << "\taddiu $sp, $sp, -4" << std::endl;
	for (std::vector<Expression *>::reverse_iterator it = this->args.rbegin(); it != args.rend(); it++) {
		(*it)->codeGen(out);
		out << "\tsw $a0, 0($sp)" << std::endl;
		out << "\taddiu $sp, $sp, -4" << std::endl;
	}
	out << "\tjal " << "func" << this->identifier << std::endl;
}

void Var::codeGen(std::ostream &out) {
	if (this->dv->i == -1) {
		out << "\tlw $a0," << "var" + name << std::endl;
	}
	else
		out << "\tlw $a0," << -4 * (this->dv->i + 1) << "($fp)" << std::endl;
}

void Assignment::codeGen(std::ostream &out) {
	this->value.codeGen(out);
	if (this->dv->i == -1)
		out << "\tsw $a0," << "var" + this->identifier << std::endl;
	else
		out << "\tsw $a0," << -4 * (this->dv->i + 1) << "($fp)" << std::endl;
}

void Return::codeGen(std::ostream &out) {
	this->value.codeGen(out);
	out << "\tmove $v0, $a0" << std::endl;
	out << "\tj " << "end_"  << currfunc << std::endl;
}
