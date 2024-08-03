#include <iostream>
#include <string>
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
