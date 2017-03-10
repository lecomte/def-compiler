parser: lexer.o parser.o check.o
	g++ -g -std=c++11 lexer.o check.o parser.o -o parser
	
lexer.o: parser.h lexer.cpp
	g++ -g -std=c++11 -c lexer.cpp
	
parser.o: parser.cpp
	g++ -g -std=c++11 -c parser.cpp
	
check.o: check.cpp
	g++ -g -std=c++11 -c check.cpp
	
parser.cpp: parser.y
	bison -d -o parser.cpp parser.y
	
parser.h: parser.cpp

lexer.cpp: teste.l
	lex -o lexer.cpp teste.l
	
clean:
	rm *.o *.cpp
