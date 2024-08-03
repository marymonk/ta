#include <iostream>
#include <string>
#include <set>
#include <map>
#include "tokens.h"

extern "C" int yylex();
extern "C" char* yytext;

std::map<std::string, std::set<std::string>> classHierarchy;
std::map<std::string, std::set<std::string>> classFunctions;

void parseClass() {
    std::string className, parentClass;
    std::set<std::string> functions;
    int token = yylex();
    if (token == IDENTIFIER) {
        className = yytext;
    }
    else {
        std::cerr << "Error: expected class name" << std::endl;
        return;
    }
    token = yylex();
    if (token == COLON) {
        token = yylex();
        if (token == IDENTIFIER) {
            parentClass = yytext;
        }
        else {
            std::cerr << "Error: expected parent class name" << std::endl;
            return;
        }
    }
    token = yylex();
    if (token == LBRACE) {
        while ((token = yylex()) == IDENTIFIER) {
            std::string functionName = yytext;
            token = yylex();
            if (token == LPAREN) {
                token = yylex();
                if (token == RPAREN) {
                    functions.insert(functionName);
                }
                else {
                    std::cerr << "Error: expected ')'" << std::endl;
                    return;
                }
            }
            else {
                std::cerr << "Error: expected '('" << std::endl;
                return;
            }
        }
        if (token != RBRACE) {
            std::cerr << "Error: expected '}'" << std::endl;
            return;
        }
        token = yylex();
        if (token != SEMICOLON) {
            std::cerr << "Error: expected ';'" << std::endl;
            return;
        }
    }
    else {
        std::cerr << "Error: expected '{'" << std::endl;
        return;
    }

    classHierarchy[className].insert(parentClass);
    classFunctions[className] = functions;
}

int main() {
    while (true) {
        int token = yylex();
        if (token == 0) break;
        if (token == CLASS) {
            parseClass();
        }
    }

    std::map<std::string, std::set<std::string>> functionMap;
    for (const auto& entry : classFunctions) {
        const std::string& className = entry.first;
        const std::set<std::string>& functions = entry.second;
        for (const auto& functionName : functions) {
            functionMap[functionName].insert(className);
        }
    }

    for (const auto& entry : functionMap) {
        if (entry.second.size() > 1) {
            std::cout << "Function: " << entry.first << std::endl;
            for (const auto& className : entry.second) {
                std::cout << "  Class: " << className << std::endl;
            }
        }
    }

    return 0;
}
