#ifndef CHARNODE_H
#define CHARNODE_H

#include <ASTNode.h>

namespace abstractTree {
    class CharNode : public ASTNode { //���� ������, ���������� ������
    private:
        char smb = '\0';// ������ ������, ������� ������������ ���� ����
        size_t number = 0;//����� ����
    public:
        CharNode() = default;
        CharNode(char smb);

        std::shared_ptr<ASTNode> copy() const override;

        void print(std::string prefix) const override;

        bool Nullable() const override;
        std::set<size_t> Firstpos() const override; //���������� ��������� ������� ������� � ���������� ���� 
        std::set<size_t> Lastpos() const override;

        void enumerate(size_t num);//��������� ������

        char getValue() const;
    };
}
#endif //CHARNODE_H