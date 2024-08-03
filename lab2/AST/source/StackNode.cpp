#include <StackNode.h>
#include <CharNode.h>

namespace abstractTree {
    std::shared_ptr<OperationNode> StackRepeatNode::compile(std::vector<std::shared_ptr<ASTNode>> args) {
        if (args.size() != getArgsCount())
            throw std::invalid_argument("wrong count of operands");
        
        auto current = args[0];
        std::shared_ptr<OperationNode> result(nullptr);

        if (count == 1)
            result = std::make_shared<Concatenation>(current, std::make_shared<CharNode>());
        else
            result = std::make_shared<Concatenation>(current, current->copy());

        for (int i = 2; i < count; ++i) {
            result = std::make_shared<Concatenation>(result, current->copy());
        }
        return result;
    }

    std::shared_ptr<OperationNode> StackOrNode::compile(std::vector<std::shared_ptr<ASTNode>> args) {
        if (args.size() != getArgsCount())
            throw std::invalid_argument("wrong count of operands");
        return std::make_shared<Or>(args[0], args[1]);
    }

    std::shared_ptr<OperationNode> StackConcatinationNode::compile(std::vector<std::shared_ptr<ASTNode>> args) {
        if (args.size() != getArgsCount())
            throw std::invalid_argument("wrong count of operands");
        return std::make_shared<Concatenation>(args[0], args[1]);
    }
    
    std::shared_ptr<OperationNode> StackKliniClosureNode::compile(std::vector<std::shared_ptr<ASTNode>> args) {
        if (args.size() != getArgsCount())
            throw std::invalid_argument("wrong count of operands");
        return std::make_shared<KliniClosure>(args[0]);
    }

    std::shared_ptr<OperationNode> Bracket::compile(std::vector<std::shared_ptr<ASTNode>> args) {
        return nullptr;
    }

    std::shared_ptr<OperationNode> StackCatchGroupNode::compile(std::vector<std::shared_ptr<ASTNode>> args) {
        if (args.size() != getArgsCount())
            throw std::invalid_argument("wrong count of operands");
        return std::make_shared<CatchGroup>(name, args[0]);
    }

    std::shared_ptr<OperationNode> StackOptionalNode::compile(std::vector<std::shared_ptr<ASTNode>> args) {
        if (args.size() != getArgsCount())
            throw std::invalid_argument("wrong count of operands");
        return std::make_shared<Or>(args[0], std::make_shared<CharNode>());
    }
}