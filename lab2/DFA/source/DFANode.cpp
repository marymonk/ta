#include <DFANode.h>
#include <stdexcept>

namespace automate {
    void DFANode::setTransition(char c, DFANode &node) {
        transitions[c] = {&node,{},{}};
    }

    void DFANode::setTransition(char c, DFANode &node, const std::set<std::string> &openingGroups, const std::set<std::string> &closingGroups) {
        transitions[c] = {&node, openingGroups, closingGroups};
    }

    Transition DFANode::getTransitionInfo(char c) const {
        try {
            return transitions.at(c);
        } catch(const std::out_of_range&) {
            throw std::invalid_argument("There is no transition to the symbol");
        }
    }

    std::map<char, Transition> DFANode::getTransitions() const {
        return transitions;
    }

    const DFANode *DFANode::transfer(char c) const {
        try {
            return transitions.at(c).to;
        } catch(const std::out_of_range&) {
            return nullptr;
            // throw std::invalid_argument("There is no transition to the symbol");
        }
    }
    
    const DFANode *DefaultDFANode::transfer(char c) const {
        return this;
    }
    
}