parser: lexer.o parser.o
	g++ lexer.o parser.o -o parser
	
lexer.o: parser.h lexer.cpp
	g++ -c lexer.cpp
	
parser.o: parser.cpp
	g++ -c parser.cpp
	
parser.cpp: parser.y
	bison -d -o parser.cpp parser.y
	
parser.h: parser.cpp

lexer.cpp: teste.l
	flex -o lexer.cpp teste.l
	
clean:
	rm *.o *.cpp
