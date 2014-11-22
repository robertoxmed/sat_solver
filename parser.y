%{
#include "parser_defs.hpp"
%}
%skeleton "lalr1.cc"
%{
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
 extern void set_yy_buffer(istream& input);
%}
%union{
  formule f;
 };
%{ /* A DECLARER ABSOLUMENT APRES L'UNION */
YY_DECL;
formule lue;
%}

%token <f> ID 
%token NEG OU ET IMPL EQUIV LPAR RPAR END
%type <f> FORMULE FEQUIV FIMPL FOU FET FNEG SIMPLE FSTART

%start FORMULE
%%
FORMULE :
FSTART END {$$ = $1; lue = $$; YYACCEPT;}
;
FSTART :
FEQUIV {$$ = $1;}
;
FEQUIV : 
FIMPL {$$ = $1;}
| FIMPL EQUIV FEQUIV {$$ = equiv($1,$3);}
;
FIMPL : 
FOU {$$ = $1;}
| FOU IMPL FIMPL {$$ = impl($1,$3);}
;
FOU :
  FET {$$ = $1;}
| FET OU FOU {$$ = ou($1,$3);}
;
FET :
  FNEG {$$ = $1;}
| FNEG ET FET {$$ = et($1,$3);}
FNEG :
  SIMPLE {$$ = $1;}
| NEG FNEG {$$ = non($2);}
;
SIMPLE :
ID {$$=$1;}
| LPAR FSTART RPAR {$$=$2;}
;
%%
void yy::parser::error(yy::location const& loc, std::string const& s){
  cerr<<endl<<s<<endl;
}

formule lit_formule() {
  yy::parser* pparser = new yy::parser();
  /* pparser->set_debug_level(1); */ 
  int i = pparser->parse();
  formule f;
  if (i==0) {
    f=lue;
  } else {
    f=NULL;
  } 
  return f;
}

formule lit_formule(istream & file) {
  set_yy_buffer(file);
  return lit_formule();
}

formule lit_formule(const string & s) {
  istringstream is (s);
  set_yy_buffer(is);
  return lit_formule();
}
