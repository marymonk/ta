%define api.value.type union

%language "c"
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

extern int yylex();
extern int yyparse();
extern void yyerror(const char* s);
extern map<string, set<string>> parent_child_map;
extern map<string, set<string>> class_functions;
extern vector<pair<string, string>> valid_pairs;

string current_class;
string parent_class;
vector<string> current_functions;

void check_and_store_pairs() {
    if (!parent_class.empty()) {
        parent_child_map[parent_class].insert(current_class);
        for (const auto& func : current_functions) {
            class_functions[current_class].insert(func);
        }
    }
}

%}

%{
%define api.value.type union
%}
%token <intval> INTEGER
%token <strval> STRING

%token CLASS "class"
%token <str> IDENTIFIER
%token ACCESS_SPECIFIER
%token COLON ":"
%token BRACE_OPEN "{"
%token BRACE_CLOSE "}"
%token PAREN_OPEN "("
%token PAREN_CLOSE ")"
%token SEMICOLON ";"
%token <str> FUNCTION
%%
program:
    /* empty */
    | program class_def
    ;

class_def:
    CLASS IDENTIFIER class_inheritance BRACE_OPEN function_list BRACE_CLOSE SEMICOLON {
        current_class = *$2;
        delete $2;
        check_and_store_pairs();
    }
    ;

class_inheritance:
    /* empty */ {
        parent_class = "";
    }
    | COLON access_specifier IDENTIFIER {
        parent_class = *$3;
        delete $3;
    }
    | COLON IDENTIFIER {
        parent_class = *$2;target_include_directories(ClassParser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    }
access_specifier:
    ACCESS_SPECIFIER
    ;

function_list:
    /* empty */
    | function_list FUNCTION SEMICOLON {
        current_functions.push_back(*$2);
        delete $2;
    }
    ;
%%

int main() {
    cout << "Enter class definitions (end with EOF):" << endl;
    yyparse();

    cout << "\nValid class pairs with at least one matching function:" << endl;
    for (const auto& parent_child : parent_child_map) {
        const string& parent = parent_child.first;
        const set<string>& children = parent_child.second;
        for (const auto& child : children) {
            const set<string>& parent_funcs = class_functions[parent];
            const set<string>& child_funcs = class_functions[child];
            vector<string> matching_funcs;
            set_intersection(parent_funcs.begin(), parent_funcs.end(),
                             child_funcs.begin(), child_funcs.end(),
                             back_inserter(matching_funcs));
            if (!matching_funcs.empty()) {
                cout << parent << " - " << child << " (functions: ";
                for (const auto& func : matching_funcs) {
                    cout << func << " ";
                }
                cout << ")" << endl;
            }
        }
    }

    return 0;
}

void yyerror(const char* s) {
    cerr << "Error: " << s << endl;
}
