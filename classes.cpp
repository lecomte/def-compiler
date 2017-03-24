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
	out << "\tlw $a0, 4($sp)" << std::endl;
	out << "\taddiu $sp, $sp, 4" << std::endl;
	out << "\tli $v0, 1" << std::endl;
	out << "\tsyscall" << std::endl;
	out << "\tli $a0, 10" << std::endl;
	out << "\tli $v0, 11" << std::endl;
	out << "\tsyscall" << std::endl;
	out << "\tjr $ra" << std::endl;
}
