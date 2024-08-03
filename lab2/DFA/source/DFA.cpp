#include <DFA.h>
#include <DFANode.h>

#include <queue>
#include <algorithm>

namespace automate {
    DFA::DFA(const abstractTree::AstEnumData& enumeratedData) {
        std::queue<std::set<size_t>> toCompute;
        std::map<std::set<size_t>, std::shared_ptr<DFANode>> states;

        auto lastPos = enumeratedData.folowPos.size()+1;
        
        states[enumeratedData.rootFirstpos] = std::make_shared<DFANode>(enumeratedData.rootFirstpos.count(lastPos));
        startNode = states.begin()->second.get();
        toCompute.push(states.begin()->first);

        while (!toCompute.empty()) {
            auto currentSet = toCompute.front();
            toCompute.pop();
            auto currentNode = states.at(currentSet);

            for (auto character : enumeratedData.character_index) {
                std::set<size_t> containedPositions;

                // позиция, присутствующая как среди позиций состояния и так и среди позиций символа
                std::set_intersection(  currentSet.begin(), currentSet.end(), 
                                        character.second.begin(), character.second.end(),
                                        std::inserter(containedPositions, containedPositions.begin()));
            
                if (!containedPositions.empty()) {
                    // определение состояния в которое происходит переход
                    std::set<size_t> nextStateSet;
                    for (auto& position : containedPositions) {
                        nextStateSet.insert( enumeratedData.folowPos[position-1].begin(), 
                                            enumeratedData.folowPos[position-1].end());
                    }
                    
                    // добавление состояния в граф, если его там нет
                    if (!states.count(nextStateSet)) {
                        states[nextStateSet] = std::make_shared<DFANode>(nextStateSet.count(lastPos));
                        toCompute.push(nextStateSet);
                    }

                    // добавление переходов (с метками групп захвата)
                    std::set<std::string> openingGroups;
                    std::set<std::string> closingGroups;

                    for (auto& position : containedPositions) {
                        for (auto& group : enumeratedData.groupsData) {
                            if (group.second.beginWith.contains(position))
                                openingGroups.insert(group.first);
                            if (group.second.endWith.contains(position))
                                closingGroups.insert(group.first);
                        }
                    }

                    currentNode->setTransition(character.first, *states[nextStateSet], openingGroups, closingGroups);
                }
            }
        }
        
        for (auto& state : states) {
            nodes.insert(state.second);
        }
        defaultNode = std::make_shared<DefaultDFANode>(false);
        groupsData = enumeratedData.groupsData;
    }

    bool DFA::match(const std::string& input) const {
        const auto *current = startNode;
        for (auto i = input.cbegin(); i != input.cend(); ++i) {
            auto next = current->transfer(*i);
            if (next == nullptr) {
                current = defaultNode.get();
            } else {
                current = next;
            }
        }
        return current->isFinal();;
    }

    bool DFA::match(const std::string &input, groupMatcher &matcher) const {
        std::map<std::string, std::string> catched;
        std::map<std::string, bool> opened;

        for (auto &i : groupsData) {
            catched[i.first] = "";
            opened[i.first] = false;
        }
        
        const auto *current = startNode;
        for (auto i = input.cbegin(); i != input.cend(); ++i) {
            auto transition = current->getTransitionInfo(*i);
            auto next = transition.to;
            if (next == nullptr) {
                current = defaultNode.get();
            } else {
                for (auto& grp : transition.openingGroups) {
                    opened[grp] = true;
                }
                for (auto& grp : opened) {
                    if (grp.second)
                        catched[grp.first] += *i;
                }
                for (auto& grp : transition.closingGroups) {
                    if (opened[grp]) {
                        matcher.setCatched(grp, catched[grp]);
                        catched[grp] = "";
                        opened[grp] = false;
                    }
                }
                current = next;
            }
        }
        return current->isFinal();;
    }

    const DFA *DFA::subtraction(const DFA& subtrahend) const {
        std::map<std::pair<const DFANode*, const DFANode*>, std::shared_ptr<DFANode>> newDFANodes;
        std::set<DFANode*> thisNodes;
        std::set<DFANode*> subtrahendNodes;

        for (auto &i : this->nodes) {
            thisNodes.insert(i.get());
        }
        thisNodes.insert(this->defaultNode.get());

        for (auto &j : subtrahend.nodes) {
            subtrahendNodes.insert(j.get());
        }
        subtrahendNodes.insert(subtrahend.defaultNode.get());

        // состояния "произведения автоматов"
        for (auto &i : thisNodes) {
            for (auto &j : subtrahendNodes) {
                newDFANodes[{i, j}] = std::make_shared<DFANode>(i->isFinal() && !j->isFinal());
            }
        }

        auto newDFA = new DFA();
        // переходы "произведения автоматов"
        for (auto &i : thisNodes) {
            auto thisTr = i->getTransitions();
            for (auto &j : subtrahendNodes) {
                auto subtrahendTr = j->getTransitions(); 
                std::map<char, Transition> newTr = thisTr;
                newTr.insert(subtrahendTr.begin(), subtrahendTr.end());

                for (auto& tr : newTr) {
                    auto thisNext = i->transfer(tr.first);
                    auto subtrahendNext = j->transfer(tr.first);

                    if (thisNext == nullptr)
                        thisNext = this->defaultNode.get();
                    if (subtrahendNext == nullptr)
                        subtrahendNext = subtrahend.defaultNode.get();

                    newDFANodes[{i,j}]->setTransition(tr.first, *newDFANodes.at({thisNext, subtrahendNext}).get());
                }
            }
        }
        
        auto newStart = newDFANodes.at({this->startNode, subtrahend.startNode});
        newDFA->startNode = newStart.get();

        // удаление недостижимых из начала
        std::queue<DFANode*> q;
        std::set<DFANode*> ns;
        q.push(newStart.get());
        ns.insert(newStart.get());
        while (!q.empty()) {
            auto current = q.front();
            q.pop();
            for (auto& tr : current->getTransitions()) {
                if (!ns.contains(tr.second.to)) {
                    q.push(tr.second.to);
                    ns.insert(tr.second.to);
                }
            }
        }
        for (auto& i : newDFANodes) {
            if (ns.contains(i.second.get())) {
                newDFA->nodes.insert(i.second);
            }
        }
        newDFA->defaultNode = std::make_shared<DefaultDFANode>(newDFANodes.at({this->defaultNode.get(),subtrahend.defaultNode.get()})->isFinal());
        return newDFA;
    }

    const DFA *DFA::addition() const {
        std::map<DFANode*, std::shared_ptr<DFANode>> ns;
        // инвернтирование принимающих и непринимающих состояний
        for (auto& n : nodes)
            ns[n.get()] = std::make_shared<DFANode>(!n->isFinal());
        
        for (auto& n : nodes) {
            for (auto& tr : n->getTransitions()) {
                ns[n.get()]->setTransition(tr.first, *ns[tr.second.to]);
            }
        }
        auto newDFA = new DFA();

        for (auto& n : ns) {
            newDFA->nodes.insert(n.second);
        }
        newDFA->startNode = ns[startNode].get();
        newDFA->defaultNode = std::make_shared<DefaultDFANode>(!defaultNode->isFinal());
        return newDFA;
    }

    const DFA *DFA::minimizate() const {
        std::set<const DFANode*> nodes_;
        for (auto &n:nodes) {
            nodes_.insert(n.get());
        }

        std::set<const DFANode*> groups;

        // помечаем состояние, переходы между которыми относятся к группам захвата
        std::queue<const DFANode*> q;
        std::set<const DFANode*> visited;
        q.push(startNode);
        while (!q.empty()) {
            auto current = q.front();
            q.pop();
            for (auto& tr : current->getTransitions()) {
                if (!visited.contains(tr.second.to)) {
                    q.push(tr.second.to);
                    visited.insert(tr.second.to);
                }
                if (!tr.second.openingGroups.empty() || !tr.second.closingGroups.empty()) {
                    groups.insert(current);
                    groups.insert(tr.second.to);
                }
            }
        }

        for (auto & n : groups) {
            nodes_.erase(n);
        }

        // основной алгоритм

        std::set<const DFANode*> F;     // все принимающие
        std::set<const DFANode*> S_F;   // все, кроме принимающих

        for (auto &n : nodes_) {
            if(n->isFinal())
                F.insert(n);
            else
                S_F.insert(n);
        }

        if (defaultNode->isFinal())
            F.insert(defaultNode.get());
        else
            S_F.insert(defaultNode.get());

        std::set<std::set<const DFANode*>> P;   // множество групп состояний по доступности
        if (!F.empty())
            P.insert(F);
        if (!S_F.empty())
            P.insert(S_F);

        for (auto &s : groups) {
            P.insert({s});
        }

        std::queue<std::set<const DFANode*>> S;
        for (auto &p : P) {
            S.push(p);
        }

        // разбиение множеств
        while (!S.empty()) {
            auto C = S.front(); S.pop();
            for (char a = ' '; a <= '~'; ++a) {
                if (a == '%') continue;
                std::map<std::set<const DFANode*>, std::set<std::set<const DFANode*>>> replaces;
                for (auto &R : P) {
                    std::set<const DFANode*> R1, R2;
                    for (auto &s : R) {
                        auto next = s->transfer(a);
                        if (!next) next = defaultNode.get();
                        if (C.contains(next))
                            R1.insert(s);  
                        else
                            R2.insert(s);
                    }
                    if (!R1.empty() && !R2.empty()) {
                        replaces[R].insert(R1);
                        replaces[R].insert(R2);
                        S.push(R1);
                        S.push(R2);
                    }
                }
                for (auto &r : replaces) {
                    P.erase(r.first);
                    P.insert(r.second.begin(), r.second.end());
                }
            }
        }
        
        // заполнение состояний минимизированного автомата
        std::map<std::set<const DFANode*>, std::shared_ptr<DFANode>> newNodes;
        std::shared_ptr<DefaultDFANode> newDefault = nullptr;

        for (auto &s : P) {
            auto isFinal = std::any_of(s.begin(), s.end(), [] (const DFANode *n) {return n->isFinal();});
            if (s.contains(defaultNode.get()))
                newDefault = std::make_shared<DefaultDFANode>(isFinal);
            else
                newNodes[s] = std::make_shared<DFANode>(isFinal);
        }

        // заполнение переходов 
        for (auto &s : P) {
            if (s.contains(defaultNode.get()))
                continue;

            auto node = *s.begin();
            for (char a = ' '; a <= '~'; ++a) {
                if (a == '%') continue;
                if (!node->transfer(a))
                    continue;
                auto tr = node->getTransitionInfo(a);
                std::set<const DFANode*> next_s;
                for (auto &s_ : P) {
                    if (s_.contains(tr.to)) {
                        next_s = s_;
                        break;
                    }
                }
                newNodes[s]->setTransition(a, *newNodes[next_s], tr.openingGroups, tr.closingGroups);
            }
        }
        // установка старнового состояния и состояние "по умолчанию"
        DFANode *newStart = nullptr;
        for (auto &s : P) {
            if (s.contains(startNode)) {
                newStart = newNodes[s].get();
                break;;
            }
        }
        auto newDFA = new DFA();
        newDFA->startNode = newStart;
        newDFA->defaultNode = newDefault;
        for (auto &n : newNodes) {
            newDFA->nodes.insert(n.second);
        }

        return newDFA;
    }

    std::string DFA::toRegex() const {
        std::map<const DFANode *, std::map<const DFANode *,std::string>> dfa;
        std::map<const DFANode *, std::set<const DFANode *>> dfa_inversed;

        // Установка связей и (прямых и обратных) и разметка рёбер
        for (auto &n : nodes) {
            for (char a = ' '; a <= '~'; ++a) {
                if (a == '%') continue;
                
                auto next = n->transfer(a);
                if (!next)
                    next = defaultNode.get();

                std::string to_add = std::string({a});
                if (abstractTree::isSpecialSMB(a))
                    to_add = "%" + to_add + "%";
                if (!dfa[n.get()].contains(next))
                    dfa[n.get()][next] = to_add;
                else 
                    dfa[n.get()][next] += "|" + to_add;

                dfa_inversed[next].insert(n.get());
            }
        }

        std::string result = "";

        // нахождение всех промежуточных состояний 
        std::queue<const DFANode*> to_exclude;
        std::queue<const DFANode*> to_compute;
        std::set<const DFANode*> visited;
        to_compute.push(startNode);
        visited.insert(startNode);
        while (!to_compute.empty()) {
            auto current = to_compute.front();to_compute.pop();

            for (auto& tr : current->getTransitions()) {
                auto n = tr.second.to;
                if (!visited.contains(n)) {
                    to_compute.push(n);
                    visited.insert(n);
                    // добавление всех промежуточных состояний в очередь на исключение
                    if (dfa_inversed.contains(n) && !(dfa_inversed[n].size() == 1 && dfa_inversed[n].contains(n)) && dfa.contains(n))
                        to_exclude.push(n);
                }
            }
        }
        // исключение всех промежуточных состояний, с свёрткой транзитивных переходов в один
        while (!to_exclude.empty()) {
            auto central = to_exclude.front(); to_exclude.pop();
            for (auto &previous : dfa_inversed[central]) {
                if (central == previous)
                    continue;
                for (auto &next : dfa[central]) {
                    if (next.first == central)
                        continue;
                    
                    std::string cycle = "";
                    if (dfa[central].contains(central))
                        cycle ="(" + dfa[central][central] + ")*";
                    
                    if (dfa[previous].contains(next.first))
                        dfa[previous][next.first] += "|";
                    
                    dfa[previous][next.first] += "((" + dfa[previous][central] + ")" + cycle + "("+ dfa[central][next.first] + "))";
                
                    // если промежуточное удаляемое состояние тоже принимающее - добавляем
                    if (central->isFinal()) {
                        if (!result.empty())
                            result += "|";
                        result += "((" + dfa[previous][central] +")" + cycle + ")";
                    }
                }
            }
        }

        // нахождение регулярного выражения по переходам из стартового в конечные
    
        /// Ребро из страртового - в стартовое
        std::string R = (dfa[startNode].contains(startNode))? (dfa[startNode][startNode]):"";
        if (startNode->isFinal()) {
            if (!result.empty())
                result += "|";
            result += "(" + R + ")*";
        }
        for (auto &tr1 : dfa[startNode]) {
            auto next = tr1.first;
            if (startNode == next)
                continue;
            if (next->isFinal()) {
                /// Ребро из страртового - в конечное
                std::string S;
                /// Ребро из конечного - в конечное
                std::string U;
                /// Ребро из конечного - в стартовое
                std::string T;
                S = tr1.second;
                U = (dfa[next].contains(next))? ("(" + dfa[next][next] + ")*"):"";
                T = (dfa[next].contains(startNode))? dfa[next][startNode]:"";

                std::string cycle = "";
                if (!T.empty()) {
                    cycle += "(" + S + U + T + ")";
                }
                if (!cycle.empty()) {
                    cycle += "|";
                    cycle += "(" + R + ")";
                }
                if (!cycle.empty())
                    cycle = "(" +cycle + ")*";

                if (!result.empty())
                    result += "|";

                result += "(";
                result += cycle + S+U;
                result += ")";
            }
        }
        return result;
    }

    void DFA::print() const {
        std::cout << "Start:" << startNode <<std::endl;
        for (auto s : nodes) {
            std::cout << s.get() << '(' <<s->isFinal() << ')' <<std::endl;
            s->print();
        }
    }

    const std::string& groupMatcher::getCatched(const std::string &name) const {
        try {
            return catched.at(name);
        } catch (const std::out_of_range&) {
            throw std::invalid_argument("Undefined group");
        }
    }
    void groupMatcher::setCatched(const std::string &name, const std::string &content) {
        catched[name] = content;
    }
}