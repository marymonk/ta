%{
#include "tokens.h"
%}

%%

"class"           { return CLASS; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return IDENTIFIER; }
"private"         { return PRIVATE; }
"protected"       { return PROTECTED; }
"public"          { return PUBLIC; }
":"               { return COLON; }
"{"               { return LBRACE; }
"}"               { return RBRACE; }
";"               { return SEMICOLON; }
"()"              { return LPAREN RPAREN; }
[ \t\n]+          { /* игнорировать пробелы и переносы строк */ }
.                 { /* игнорировать любые другие символы */ }

%%

int yywrap(void) {
    return 1;
}
