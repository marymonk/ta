#ifndef DFA_H
#define DFA_H

#include <string>
#include <set>
#include <memory>
#include <vector>

#include <AST.h>
#include <DFANode.h>

namespace automate {
    class groupMatcher {
    private:
        std::map<std::string, std::string> catched;
    public:
        groupMatcher() = default;
        void clear() {catched.clear();}

        const std::string& getCatched(const std::string& name) const;
        void setCatched(const std::string& name, const std::string& content);
    };

    class DFA {
    private:
        std::set<std::shared_ptr<DFANode>> nodes;
        DFANode* startNode;
        std::shared_ptr<DefaultDFANode> defaultNode;

        std::map<std::string, abstractTree::GroupEnumData> groupsData;
        DFA() = default;
    public:
        DFA(const abstractTree::AstEnumData& enumeratedData);

        bool match(const std::string& input) const;
        bool match(const std::string& input, groupMatcher& matcher) const;

        const DFA *subtraction(const DFA& subtrahend) const;
        const DFA *addition() const;

        const DFA *minimizate() const;

        std::string toRegex() const;
        
        void print() const;
        ~DFA() = default;
    };
}

#endif //DFA_H