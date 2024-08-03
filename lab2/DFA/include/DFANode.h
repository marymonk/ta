#ifndef DFANODE_H
#define DFANODE_H

#include <string>
#include <map>
#include <set>
#include <iostream>

namespace automate {
    class DFANode;
    
    struct Transition {
        DFANode *to;
        std::set<std::string> openingGroups;
        std::set<std::string> closingGroups;
    };
    

    class DFANode {
    private:
        std::map<char, Transition> transitions;
        bool final = false;
    public:
        DFANode() = default;
        DFANode(bool final): final(final) {};

        virtual ~DFANode() = default;

        virtual void setTransition(char c, DFANode& node);
        virtual void setTransition(char c, DFANode& node, const std::set<std::string>& openingGroups, const std::set<std::string>& closingGroups);

        Transition getTransitionInfo(char c) const;
        std::map<char, Transition> getTransitions() const;
        
        virtual const DFANode *transfer(char c) const;

        bool isFinal() const {return final;}

        void print() const {
            for (auto t : transitions) {
                std::cout << '\t' <<t.first << "->" << t.second.to <<std::endl;
            }
        }
    };

    class DefaultDFANode : public DFANode {
    public:
        DefaultDFANode() = default;
        DefaultDFANode(bool final) : DFANode(final) {}
        const DFANode *transfer(char c) const override;
    };
    
}

#endif //DFANODE_H