%option C++ noyywrap

%{
#include <fstream>
int mylineno = 0;
std::filebuf fb;
std::ofstream os;
%}

letras   [A-Za-z]
algarismo     [0-9]
num ({algarismo})*
var {letras}({letras}|{algarismo}|_)*
key "def"|"if"|"else"|"while"|"return"|"break"|"continue"|"int"|"void"
sym "("|"{"|"["|"]"|"}"|")"|","|";"|"="|"+"|"-"|"*"|"/"|"<"|">"|"<="|">="|"=="|"!="|"&&"|"||"|"!"

%%
{key} os << "KEY\t" << '"' << YYText() << '"' << '\n';

{sym} os << "SYM\t" << '"' << YYText() << '"' << '\n';

" "|\n|\t

"//"		{
		int c;

		while((c = yyinput()) != 0)
			{
			if(c == '\n')
				break;
			}
}

{num}  os << "DEC\t" << '"' << YYText() << '"' << '\n';

{var}    os << "ID\t" << '"' << YYText() << '"' << '\n';

.	os << "ERROR\t" << '"' << YYText() << '"' << '\n'; 

%%

int main( int /* argc */, char**  argv )
	{
	fb.open(argv[1], std::ios::in);
	os.open(argv[2]);
	std::istream is(&fb);
	FlexLexer* lexer = new yyFlexLexer;
	lexer->yy_switch_to_buffer(lexer->yy_create_buffer(&is,YY_BUF_SIZE));
	while(lexer->yylex() != 0)
		;
	fb.close();
	os.close();
	return 0;
}
