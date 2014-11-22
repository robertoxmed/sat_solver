%{
#include "parser_defs.hpp"
#include "parser.tab.hpp"
#include <iostream>
YY_DECL;
#define yyterminate() return yy::parser::token::END
 istream * lex_input;
#define YY_INPUT(buf,result,max_size) \
 { \
   int c = lex_input->get(); \
   result = (c == -1) ? YY_NULL : (buf[0] = c, 1); \
 } 
%}

ID       [a-z][a-z0-9]*
%%
[ \t]
"\n" {return yy::parser::token::END; }
"~"  {return yy::parser::token::NEG; }
"\\/" {return yy::parser::token::OU;}
"/\\" {return yy::parser::token::ET;}
"=>" {return yy::parser::token::IMPL;}
"<=>" {return yy::parser::token::EQUIV;}
"(" {return yy::parser::token::LPAR;}
")" {return yy::parser::token::RPAR;}
{ID} {pyylval->f=variable(yytext);return yy::parser::token::ID;}
%%
int yywrap(){return 1;}

YY_BUFFER_STATE current_state;

void set_yy_buffer(istream & input) {
  lex_input = &input;
}

