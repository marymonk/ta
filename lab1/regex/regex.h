#ifndef TA_REGEX
#define TA_REGEX

#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <vector>

struct ClassInfo {
public:
    std::string name;
    std::string accessSpecifier;
    std::string parent;
    std::vector<std::string> functions;

    void processInput();
    void printClassPairs();

private:
    std::map<std::string, ClassInfo> classMap;
};

#endif //TA_REGEX