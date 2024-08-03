#ifndef ASTNODE_H
#define ASTNODE_H

#include <string>
#include <memory>//��� ����� ���������� (���������� ��������, ������� ������, ������� == 0 -> ������ ���������)
#include <set>
#include <vector>
namespace abstractTree {
    //�������� ������������ ��������������� ������
    class ASTNode {
    public:
        ASTNode() = default;

        virtual bool Nullable() const = 0;
        virtual std::set<size_t> Firstpos() const = 0;// set - ��������� - ������������� ��������� ���������� ���������
        virtual std::set<size_t> Lastpos() const = 0;

        virtual std::shared_ptr<ASTNode> copy() const = 0;
        virtual void print(std::string prefix) const = 0;
        
        virtual ~ASTNode() = default;
    };
}
#endif //ASTNODE_H