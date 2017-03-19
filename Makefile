compiler: lexer.o parser.o check.o compiler.o
	g++ -g -std=c++11 lexer.o check.o parser.o compiler.o -o compiler
	
lexer.o: parser.hpp lexer.cpp
	g++ -g -std=c++11 -c lexer.cpp
	
parser.o: parser.cpp
	g++ -g -std=c++11 -c parser.cpp
	
check.o: check.cpp
	g++ -g -std=c++11 -c check.cpp

compiler.o: compiler.cpp
	g++ -g -std=c++11 -c compiler.cpp
	
parser.cpp: parser.y
	bison -d -o parser.cpp parser.y
	
parser.hpp: parser.cpp

lexer.cpp: teste.l
	lex -o lexer.cpp teste.l
	
clean:
	rm *.o parser.cpp parser.hpp lexer.cpp compiler
