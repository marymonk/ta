#ifndef CHARNODE_H
#define CHARNODE_H

#include <ASTNode.h>

namespace abstractTree {
    class CharNode : public ASTNode { //узел дерева, содержащий символ
    private:
        char smb = '\0';// хранит символ, который представляет этот узел
        size_t number = 0;//номер узла
    public:
        CharNode() = default;
        CharNode(char smb);

        std::shared_ptr<ASTNode> copy() const override;

        void print(std::string prefix) const override;

        bool Nullable() const override;
        std::set<size_t> Firstpos() const override; //возвращает множество позиций первого и последнего узла 
        std::set<size_t> Lastpos() const override;

        void enumerate(size_t num);//установка номера

        char getValue() const;
    };
}
#endif //CHARNODE_H