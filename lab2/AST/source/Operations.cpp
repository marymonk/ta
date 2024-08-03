#include <Operations.h>

namespace abstractTree {
    void OperationNode::print(std::string prefix) const {
        auto args = getArguments();//возвращает вектор аргументов операции
        std::cout << prefix << "Operation: " << getOperation() << std::endl; //возвращает строковое представление операции
        for (size_t i = 0; i < args.size(); ++i) {
            std::cout << prefix << "Agr: " << i <<std::endl;
            args[i]->print(prefix + "\t");
        }
    }

    Or::Or(std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) : OperationNode(), left(left), right(right) {}//инициализирует левое и правое поддеревья
    void Or::calculateNFL() {
        nullable = left->Nullable() || right->Nullable();
        //объединяем множества левого и правого поддеревьев
        auto lf = left->Firstpos();
        firstpos.insert(lf.begin(), lf.end());
        auto rf = right->Firstpos();
        firstpos.insert(rf.begin(), rf.end());

        auto ll = left->Lastpos();
        lastpos.insert(ll.begin(), ll.end());
        auto rl = right->Lastpos();
        lastpos.insert(rl.begin(), rl.end());
    }
    std::string Or::getOperation() const { 
        return "|";
    }
    const std::vector<std::shared_ptr<ASTNode>> Or::getArguments() const {
        return {left, right};
    }
    std::shared_ptr<ASTNode> Or::copy() const {
        return std::make_shared<Or>(left->copy(), right->copy());
    }

    Concatenation::Concatenation(std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) : OperationNode(), left(left), right(right) {}
    void Concatenation::calculateNFL() {
        nullable = left->Nullable() && right->Nullable();
        
        auto lf = left->Firstpos();
        firstpos.insert(lf.begin(), lf.end());
        if (left->Nullable()){
            auto rf = right->Firstpos();
            firstpos.insert(rf.begin(), rf.end());
        }

        auto rl = right->Lastpos();
        lastpos.insert(rl.begin(), rl.end());
        if (right->Nullable()) {
            auto ll = left->Lastpos();
            lastpos.insert(ll.begin(), ll.end());
        }
    }
    std::string Concatenation::getOperation() const {
        return ".";
    }
    const std::vector<std::shared_ptr<ASTNode>> Concatenation::getArguments() const {
        return {left, right};
    }
    std::shared_ptr<ASTNode> Concatenation::copy() const {
        return std::make_shared<Concatenation>(left->copy(), right->copy());
    }

    KliniClosure::KliniClosure(std::shared_ptr<ASTNode> left) : OperationNode(), left(left) {}
    void KliniClosure::calculateNFL() {
        nullable = true;
        
        auto lf = left->Firstpos();
        firstpos.insert(lf.begin(), lf.end());

        auto ll = left->Lastpos();
        lastpos.insert(ll.begin(), ll.end());
    }
    std::string KliniClosure::getOperation() const {
        return "*";
    }
    const std::vector<std::shared_ptr<ASTNode>> KliniClosure::getArguments() const {
        return {left};
    }
    std::shared_ptr<ASTNode> KliniClosure::copy() const {
        return std::make_shared<KliniClosure>(left->copy());
    }

    CatchGroup::CatchGroup(const std::string &name, std::shared_ptr<ASTNode> left) : OperationNode(), name(name), left(left) {}
    void CatchGroup::calculateNFL()
    {
        nullable = left->Nullable();
        
        auto lf = left->Firstpos();
        firstpos.insert(lf.begin(), lf.end());

        auto ll = left->Lastpos();
        lastpos.insert(ll.begin(), ll.end());
    }
    std::string CatchGroup::getOperation() const {
        return "(" + name + ")";
    }
    const std::vector<std::shared_ptr<ASTNode>> CatchGroup::getArguments() const {
        return {left};
    }
    std::string CatchGroup::getName() const {
        return name;
    }
    std::shared_ptr<ASTNode> CatchGroup::copy() const {
        return std::make_shared<CatchGroup>(name, left->copy());
    }
}