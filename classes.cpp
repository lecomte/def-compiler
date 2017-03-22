#include <bits/stdc++.h>
#include "classes.hpp"

void Program::codeGen(std::ofstream &out) {
	out << ".data" << std::endl;
	out << ".align 2" << std::endl;
	for (Declaration *d : decs) {
		if (DecVar *dv = dynamic_cast<DecVar *>(d)) {
			out << 	dv->identificator << ": .space 4" << std::endl;
		}
	}
	out << ".text" << std::endl;
	for (Declaration *d : decs) {
		if (DecFunc *df = dynamic_cast<DecFunc *>(d)) {
			if (df->identificator == "main") {
				out << "main:" << std::endl;
				out << "\t" << "li $v0, 10" << std::endl;
				out << "\t" << "syscall" << std::endl;
			}
		}
	}
}
