%option noyywrap

%{ 
#include <string>
#include "parser.h"
%}

letras   [A-Za-z]
algarismo     [0-9]
num ({algarismo})*
var {letras}({letras}|{algarismo}|_)*
key "def"|"if"|"else"|"while"|"return"|"break"|"continue"|"int"|"void"
sym "("|"{"|"["|"]"|"}"|")"|","|";"|"="|"+"|"-"|"*"|"/"|"<"|">"|"<="|">="|"=="|"!="|"&&"|"||"|"!"

%%
"def" return DEF;
"if" return IF;
"else" return ELSE;
"while" return WHILE;
"return" return RETURN;
"break" return BREAK;
"continue" return CONTINUE;
"int" return INTEGER;
"void" return VOID;
"(" return LPAREN;
")" return RPAREN;
"{" return LBRACE;
"}" return RBRACE;
"," return COMMA;
";" return SCOL;
"=" return EQTO;
"+" return PLUS;
"-" return MINUS;
"*" return MULT;
"/" return DIV;
"<" return LESS;
">" return GRE;
"<=" return LEQ;
">=" return GEQ;
"==" return EQ;
"!=" return DIFF;
"&&" return AND;
"||" return OR;
"!" return NOT;


" "|\n|\t

"//"		{
		int c;

		while((c = yyinput()) != 0)
			{
			if(c == '\n')
				break;
			}
}

{num}  {yylval.number = &(atoi(yytext)); return NUM;}

{var}   {yylval.name = new std::string(yytext, yyleng); return IDENTIFIER;}

.	return 1;

%%
