%option noyywrap

%{ 
#include <string>
#include <vector>
#include "classes.h"
#include "parser.hpp"
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
"+" {yylval.name = new std::string(yytext, yyleng); return PLUS;}
"-" {yylval.name = new std::string(yytext, yyleng); return MINUS;}
"*" {yylval.name = new std::string(yytext, yyleng); return MULT;}
"/" {yylval.name = new std::string(yytext, yyleng); return DIV;}
"<" {yylval.name = new std::string(yytext, yyleng); return LESS;}
">" {yylval.name = new std::string(yytext, yyleng); return GRE;}
"<=" {yylval.name = new std::string(yytext, yyleng); return LEQ;}
">=" {yylval.name = new std::string(yytext, yyleng); return GEQ;}
"==" {yylval.name = new std::string(yytext, yyleng); return EQ;}
"!=" {yylval.name = new std::string(yytext, yyleng); return DIFF;}
"&&" {yylval.name = new std::string(yytext, yyleng); return AND;}
"||" {yylval.name = new std::string(yytext, yyleng); return OR;}
"!" {yylval.name = new std::string(yytext, yyleng); return NOT;}


" "|\n|\t	;

"//"		{
		int c;

		while((c = yyinput()) != 0)
			{
			if(c == '\n')
				break;
			}
}

{num}  {int n = atoi(yytext); yylval.number = n; return NUM;}

{var}   {yylval.name = new std::string(yytext, yyleng); return IDENTIFIER;}

.	return 1;

%%
