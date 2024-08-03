%{
#include "parser.tab.h"
%}

%%
"{"         { return BRACE_OPEN; }
"}"         { return BRACE_CLOSE; }
":"         { return COLON; }
"("         { return PAREN_OPEN; }
")"         { return PAREN_CLOSE; }
";"         { return SEMICOLON; }
"class"     { return CLASS; }
"private"   { return ACCESS_SPECIFIER; }
"protected" { return ACCESS_SPECIFIER; }
"public"    { return ACCESS_SPECIFIER; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.str = new std::string(yytext); return IDENTIFIER; }
[a-zA-Z_][a-zA-Z0-9_]*\(\) { yylval.str = new std::string(yytext); return FUNCTION; }
[ \t\n]+    ;
.           { yyerror("Unexpected character"); }

%%

int yywrap() {
    return 1;
}
