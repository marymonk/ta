#include <AST.h>
#include <stack>

#define ESCSMB '%'

namespace abstractTree {

static std::set<char> specialSymbols = {'|','*','(', ')', '<', '>', '|', '?', '{', '}'};

bool isSMB(char smb) {
    return ' ' <= smb && smb <= '~';
}
bool isSpecialSMB(char smb) { //проверяет является ли символ специальным
    return specialSymbols.contains(smb);
}

void pushToResult(std::stack<std::shared_ptr<ASTNode>>& resultStack, StackNode* operation) {
    size_t argumentsCount = operation->getArgsCount();
    if (argumentsCount == 0) {
        return;
    }
    
    if (resultStack.size() < argumentsCount) {
        throw std::runtime_error("Not enough arguments to operation");
    }

    std::vector<std::shared_ptr<ASTNode>> args(argumentsCount);
    for (int i = argumentsCount-1; i >= 0 ; i--) {
        args[i] = resultStack.top();
        resultStack.pop();
    }
    auto new_operation = operation->compile(args);
    resultStack.push(new_operation);
}

void addOperationToStack(std::stack<StackNode*>&operationsStack, std::stack<std::shared_ptr<ASTNode>>& resultStack, StackNode* operation) {
    if (dynamic_cast<Bracket*>(operation) == nullptr){
        auto priority = operation->getPriority();
        
        while (!(   operationsStack.empty() 
                    || dynamic_cast<Bracket*>(operationsStack.top()) != nullptr 
                    || operationsStack.top()->getPriority() < priority)) {
            pushToResult(resultStack, operationsStack.top()); 
            delete operationsStack.top();
            operationsStack.pop();
        }
    }
    operationsStack.push(operation);
}

void AST::readGroupName( const std::string& expr, size_t& i, 
                    std::stack<StackNode*>&operationsStack, 
                    std::stack<std::shared_ptr<ASTNode>>& resultStack) {
    size_t i_ = i + 1;
    if (i_ >= expr.size() || expr[i_] != '<'){
        addOperationToStack(operationsStack, resultStack, new Bracket());
        return;
    }
    i_ += 1;
    while (i_ < expr.size() && isalpha(expr[i_])) {
        ++i_;
    }
    if (!(i_ < expr.size()) || expr[i_] != '>') {
        addOperationToStack(operationsStack, resultStack, new Bracket());
        return;
    }
    
    auto name = std::string(expr.begin() + i + 2, expr.begin() + i_);
    if (groups.count(name) != 0)
        throw std::runtime_error("Multiple declaration of capture groups is prohibited");
    
    addOperationToStack(operationsStack, resultStack, new StackCatchGroupNode(name));
    groups.insert(name);

    i = i_;
}


AST::AST(const std::string &expr){
    std::stack<StackNode*> operationsStack;
    std::stack<std::shared_ptr<ASTNode>> resultStack;

    bool escaping = false;
    bool begin_of_token = true;

    for (size_t i = 0; i < expr.size(); i++) {
        auto current = expr[i];
        if (!isSMB(current))
            throw std::runtime_error("Non printable symbol");
        
        if (current == ESCSMB) {
            escaping = !escaping;
            continue;
        }

        if (escaping) {
            if (!begin_of_token)
                addOperationToStack(operationsStack, resultStack, new StackConcatinationNode());
            resultStack.push(std::make_shared<CharNode>(current));
        } else {
            if (current == '*') {
                addOperationToStack(operationsStack, resultStack, new StackKliniClosureNode());
            } else if (current == '?') {
                addOperationToStack(operationsStack, resultStack, new StackOptionalNode());
            } else if (current =='|') {
                addOperationToStack(operationsStack, resultStack, new StackOrNode());
                begin_of_token = true;
                continue;
            } else if (current == '(') {
                if (!begin_of_token)
                    addOperationToStack(operationsStack, resultStack, new StackConcatinationNode());
                readGroupName(expr, i, operationsStack, resultStack);
                begin_of_token = true;
                continue;
            } else if (current == ')') {
                if (begin_of_token)
                    throw std::runtime_error("Empty brackets");
                
                while (!operationsStack.empty() && (dynamic_cast<Bracket*>(operationsStack.top()) == nullptr)) {
                    pushToResult(resultStack, operationsStack.top());
                    delete operationsStack.top();
                    operationsStack.pop();
                }

                if (operationsStack.empty())
                    throw std::runtime_error("Wrong brackets sequence (too many closing)");

                pushToResult(resultStack, operationsStack.top());
                delete operationsStack.top();
                operationsStack.pop();
            } else if (current == '{') {
                size_t i_ = i + 1;
                while (i_ < expr.size() && isdigit(expr[i_])) {
                    ++i_;
                }
                if (!(i_ < expr.size()) || expr[i_] != '}')
                    throw std::runtime_error("Wrong repeating format");

                int n = atoi(std::string(expr.begin() + i + 1, expr.begin() + i_).c_str());
                addOperationToStack(operationsStack, resultStack, new StackRepeatNode(n));
                i = i_;

            } else {
                if (isSpecialSMB(current))
                    throw std::runtime_error("Wrong using of metasymbol");

                if (!begin_of_token)
                    addOperationToStack(operationsStack, resultStack, new StackConcatinationNode());
                resultStack.push(std::make_shared<CharNode>(current));
            }
        }
    
        begin_of_token = false;
    }
    
    if (escaping) 
        throw std::runtime_error("Escaping isn't closed");

    while (!operationsStack.empty()) {
        auto c = operationsStack.top();
        if (dynamic_cast<Bracket*>(c) != nullptr )
            throw std::runtime_error("Wrong brackets sequence (too many opening)");
        pushToResult(resultStack, c);
        delete c;
        operationsStack.pop();
    }
    root = resultStack.top();
}

size_t AST::calculateNode(ASTNode *node, size_t num) {
    if (dynamic_cast<CharNode*>(node)) {
        auto *node_ = dynamic_cast<CharNode*>(node);
        if (!node_->Nullable()) {
            node_->enumerate(num);
            enumData.folowPos.resize(num);
            enumData.character_index[node_->getValue()].insert(num);
            return num + 1;
        }
        return num;
    } else if (dynamic_cast<OperationNode*>(node)) {
        auto *node_ = dynamic_cast<OperationNode*>(node);
        for (auto& arg : node_->getArguments()) {
            num = calculateNode(arg.get(), num);
        }
        node_->calculateNFL();

        if (dynamic_cast<Concatenation*>(node)) {
            auto args = node_->getArguments();
            for (auto& last : args[0]->Lastpos()) {
                auto f = args[1]->Firstpos();
                enumData.folowPos[last-1].insert(f.begin(), f.end());
            }
        } else if (dynamic_cast<KliniClosure*>(node)) {
            auto args = node_->getArguments();
            for (auto& last : args[0]->Lastpos()) {
                auto f = args[0]->Firstpos();
                enumData.folowPos[last-1].insert(f.begin(), f.end());
            }
        } else if (dynamic_cast<CatchGroup*>(node)) {
            auto *group = dynamic_cast<CatchGroup*>(node);
            auto f = group->Firstpos();
            auto l =  group->Lastpos();
            enumData.groupsData[group->getName()].beginWith.insert(f.begin(), f.end()); 
            enumData.groupsData[group->getName()].endWith.insert(l.begin(), l.end()); 
        }
        return num;
    } else {
        throw "ERROR";
    }
    return num;
}

void AST::calculateFollowpos() {
    size_t n = 1;
    n = calculateNode(root.get(), n);
    for (auto& last : root->Lastpos()) {
        enumData.folowPos[last-1].insert(n);
    }
    enumData.rootFirstpos = root->Firstpos();
    if (root->Nullable())
        enumData.rootFirstpos.insert(n);
}

void AST::print() const {
    if (root == nullptr)
        std::cout <<"empty" <<std::endl;
    else
        root->print("");
}

AstEnumData AST::getEnumData() const {
    return enumData;
}
}