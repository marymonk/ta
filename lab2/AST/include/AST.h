#ifndef AST_H
#define AST_H

#include <string>
#include <map>
#include <memory>
#include <set>
#include <iostream>

#include <vector>
#include <stack> 
#include <StackNode.h>
#include <Operations.h>
#include <CharNode.h>
#include <ASTNode.h>

namespace abstractTree {
    bool isSMB(char smb);
    bool isSpecialSMB(char smb);

    struct GroupEnumData {//позиции в группе
        std::set<size_t> beginWith;
        std::set<size_t> insideIn;
        std::set<size_t> endWith;
    };

    struct AstEnumData {
        std::map<char, std::set<size_t>> character_index;
        std::vector<std::set<size_t>> folowPos;//вектор позиций 
        std::set<size_t> rootFirstpos;//множества позиций первого элемента корня
        std::map<std::string, GroupEnumData> groupsData;
    };


    class AST {
    private:
        std::shared_ptr<ASTNode> root;

        AstEnumData enumData;

        std::set<std::string> groups;
        std::map<std::string, GroupEnumData> grpEnumData; 

        void readGroupName( const std::string& expr, size_t& i, 
                        std::stack<StackNode*>&operationsStack, 
                        std::stack<std::shared_ptr<ASTNode>>& resultStack);

        size_t calculateNode(ASTNode* node, size_t num);

    public:
        AST(const std::string& expr);

        void calculateFollowpos();

        void print() const;

        AstEnumData getEnumData() const;
    };
}

#endif  //AST_H