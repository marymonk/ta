#include <myRegex.h>
#include <AST.h>

namespace myRegex {
    const automate::DFA* compile(const std::string &regex) {
        auto ast = abstractTree::AST(regex);
        ast.calculateFollowpos();
        return new automate::DFA(ast.getEnumData());
    }

    bool match(const std::string &regex, const std::string &input) {
        auto ast = abstractTree::AST(regex);
        ast.calculateFollowpos();
        auto compiled = automate::DFA(ast.getEnumData());
        return compiled.match(input);
    }

    bool match(const std::string &regex, const std::string &input, automate::groupMatcher &matcher) {
        auto ast = abstractTree::AST(regex);
        ast.calculateFollowpos();
        auto compiled = automate::DFA(ast.getEnumData());
        return compiled.match(input, matcher);
    }

    bool match(const automate::DFA &compiled, const std::string &input) {
        return compiled.match(input);
    }

    bool match(const automate::DFA &compiled, const std::string &input, automate::groupMatcher &matcher) {
        return compiled.match(input, matcher);
    }

    std::vector<std::string> findall(const std::string &regex, const std::string &input) {
        auto ast = abstractTree::AST(regex);
        ast.calculateFollowpos();
        auto compiled = automate::DFA(ast.getEnumData());
        return findall(compiled, input);
    }

    std::vector<std::string> findall(const std::string &regex, const std::string &input, automate::groupMatcher &matcher) {
        auto ast = abstractTree::AST(regex);
        ast.calculateFollowpos();
        auto compiled = automate::DFA(ast.getEnumData());
        return findall(compiled, input, matcher);
    }

    std::vector<std::string> findall(const automate::DFA &compiled, const std::string &input) {
        std::vector<std::string> result;
        for (size_t i = 0; i < input.size(); i++) {
            for (size_t j = 1; j <= input.size()-i; j++) {
                auto sub = input.substr(i,j);
                if (match(compiled, sub))
                    result.push_back(sub);
            }
        }
        
        return result;
    }

    std::vector<std::string> findall(const automate::DFA &compiled, const std::string &input, automate::groupMatcher &matcher){
        std::vector<std::string> result;
        for (size_t i = 0; i < input.size(); i++) {
            for (size_t j = 1; j <= input.size()-i; j++) {
                auto sub = input.substr(i,j);
                if (match(compiled, sub, matcher)) 
                    result.push_back(sub);
            }
        }        
        return result;
    }

    const automate::DFA* subtraction(const automate::DFA &base, const automate::DFA &subtrahend) {
        return base.subtraction(subtrahend);
    }

    const automate::DFA* subtraction(const std::string &baseRegex, const std::string &subtrahendRegex) {
        auto baseAst = abstractTree::AST(baseRegex);
        baseAst.calculateFollowpos();
        auto baseCompiled = automate::DFA(baseAst.getEnumData());

        auto subtrahendAst = abstractTree::AST(subtrahendRegex);
        subtrahendAst.calculateFollowpos();
        auto subtrahendCompiled = automate::DFA(subtrahendAst.getEnumData());

        return baseCompiled.subtraction(subtrahendCompiled);
    }

    const automate::DFA* addition(const std::string &regex) {
        auto ast = abstractTree::AST(regex);
        ast.calculateFollowpos();
        auto compiled = automate::DFA(ast.getEnumData());
        return compiled.addition();
    }

    const automate::DFA* addition(automate::DFA &compiled) {
        return compiled.addition();
    }
}