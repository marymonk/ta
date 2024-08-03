#ifndef TOKENS_H
#define TOKENS_H

enum yytokentype {
    CLASS = 258,
    IDENTIFIER,
    PRIVATE,
    PROTECTED,
    PUBLIC,
    COLON,
    LBRACE,
    RBRACE,
    SEMICOLON,
    LPAREN,
    RPAREN
};

extern "C" int yylex();
extern "C" int yywrap();


#endif
