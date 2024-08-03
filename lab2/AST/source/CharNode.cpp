#include <CharNode.h>
#include <iostream>

namespace abstractTree {
    CharNode::CharNode(char smb) : ASTNode(), smb(smb) {}

    std::shared_ptr<ASTNode> abstractTree::CharNode::copy() const {
        return std::make_shared<CharNode>(smb);//возвращает указатель на новый объект 
    }

    void abstractTree::CharNode::print(std::string prefix) const {
        if (smb == '\0') {
            std::cout <<prefix << "Empty" << std::endl;
        } else {
            std::cout <<prefix << smb << std::endl;
        }
    }
    
    bool CharNode::Nullable() const {
        return '\0'==smb;
    }
    std::set<size_t> CharNode::Firstpos() const {
        if (Nullable())
            return {};
        else
            return {number};
    }
    std::set<size_t> CharNode::Lastpos() const {
        if (Nullable())
            return std::set<size_t>();
        else
            return std::set<size_t>({number});
    }
    
    void CharNode::enumerate(size_t num) {
        if (num == 0)
            throw std::invalid_argument("Invalid node number");
        
        if (number != 0)
            throw std::logic_error("It is forbidden to reassign the node number");

        number = num;
    }
    char CharNode::getValue() const {
        return smb;
    }
}