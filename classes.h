#ifndef CLASSES_H
#define CLASSES_H

class Program {
	public:
		std::vector<DecVar> *decs;
		Program(std::vector<DecVar> *d) : decs(d) {}
}

#endif
