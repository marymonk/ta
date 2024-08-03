#include "regex.h"

void ClassInfo::processInput() {
    std::string input;

    std::regex classPattern("^class\\s+([a-zA-Z_][a-zA-Z0-9_]*)\\s*:\\s*(private|protected|public)?\\s*([a-zA-Z_][a-zA-Z0-9_]*)?\\s*\\{([^}]*)\\};");
    std::regex functionPattern("([a-zA-Z_][a-zA-Z0-9_]*)\\s*\\(([^)]*)\\)");
    std::regex parameterPattern("([a-zA-Z_][a-zA-Z0-9_]*)");

    std::cout << "Enter the lines according to the pattern (to complete the entry 'exit'):" << std::endl;

    while (true) {
        std::getline(std::cin, input);

        if (input == "exit") break;

        std::smatch match;
        if (std::regex_match(input, match, classPattern)) {
            ClassInfo classInfo;
            classInfo.name = match[1];
            classInfo.accessSpecifier = match[2];
            classInfo.parent = match[3];
            std::string functions = match[4];

            std::sregex_iterator funcIt(functions.begin(), functions.end(), functionPattern);
            std::sregex_iterator funcEnd;

            while (funcIt != funcEnd) {
                std::string functionName = (*funcIt)[1];
                std::string parameters = (*funcIt)[2];

                std::sregex_iterator paramIt(parameters.begin(), parameters.end(), parameterPattern);
                std::sregex_iterator paramEnd;
                std::vector<std::string> params;

                while (paramIt != paramEnd) {
                    params.push_back((*paramIt)[1]);
                    ++paramIt;
                }

                std::string functionWithParams = functionName + "(";
                for (size_t i = 0; i < params.size(); ++i) {
                    functionWithParams += params[i];
                    if (i < params.size() - 1) {
                        functionWithParams += ", ";
                    }
                }
                functionWithParams += ")";
                classInfo.functions.push_back(functionWithParams);

                ++funcIt;
            }

            classMap[classInfo.name] = classInfo;
        }
        else {
            std::cout << "The string does not match the class template." << std::endl;
        }
    }

    this->classMap = classMap;
}

void ClassInfo::printClassPairs() {
    std::cout << "Test results:" << std::endl;
    // Вложенные циклы для сравнения всех пар классов
    for (auto it1 = classMap.begin(); it1 != classMap.end(); ++it1) {
        for (auto it2 = std::next(it1); it2 != classMap.end(); ++it2) {
            for (const auto& func : it1->second.functions) {
                if (std::find(it2->second.functions.begin(), it2->second.functions.end(), func) != it2->second.functions.end()) {
                    std::cout << it1->second.name << " - " << it2->second.name << " : " << func << std::endl;
                }
            }
        }
    }
}