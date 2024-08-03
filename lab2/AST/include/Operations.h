#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "ASTNode.h"
#include <vector>
#include <memory>
#include <iostream>
#include <set>

namespace abstractTree {
class OperationNode : public ASTNode {
protected:
    bool nullable;
    std::set<size_t> firstpos;
    std::set<size_t> lastpos;

public:
    OperationNode() = default;

    virtual void calculateNFL() = 0;
    
    virtual const std::vector<std::shared_ptr<ASTNode>> getArguments() const = 0;
    virtual std::string getOperation() const = 0;
    
    void print(std::string prefix) const override;

    bool Nullable() const override {
        return nullable;
    }

    std::set<size_t> Firstpos() const override {
        return firstpos;
    }

    std::set<size_t> Lastpos() const override {
        return lastpos;
    }
};

class Or : public OperationNode {
private:
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
public:
    Or(std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right);
    
    void calculateNFL() override;
    
    std::string getOperation() const override;
    const std::vector<std::shared_ptr<ASTNode>> getArguments() const override;

    std::shared_ptr<ASTNode> copy() const override;
};

class Concatenation : public OperationNode {
private:
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
public:
    Concatenation(std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right);
        
    void calculateNFL() override;

    std::string getOperation() const override;
    const std::vector<std::shared_ptr<ASTNode>> getArguments() const override;

    std::shared_ptr<ASTNode> copy() const override;
};

class KliniClosure : public OperationNode {
private:
    std::shared_ptr<ASTNode> left;
public:
    KliniClosure(std::shared_ptr<ASTNode> left);
    
    void calculateNFL() override;

    std::string getOperation() const override;
    const std::vector<std::shared_ptr<ASTNode>> getArguments() const override;

    std::shared_ptr<ASTNode> copy() const override;
};

class CatchGroup : public OperationNode {
private:
    std::shared_ptr<ASTNode> left;
    std::string name;
public:
    CatchGroup(const std::string& name, std::shared_ptr<ASTNode> left);
    
    void calculateNFL() override;
    
    std::string getOperation() const override;
    const std::vector<std::shared_ptr<ASTNode>> getArguments() const override;
    
    std::string getName() const;

    std::shared_ptr<ASTNode> copy() const override;
};
}
#endif  //OPERATIONS_H
