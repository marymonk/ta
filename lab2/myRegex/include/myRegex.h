#ifndef MYREGEX_H
#define MYREGEX_H

#include <string>
#include <vector>
#include <DFA.h>

namespace myRegex {
    const automate::DFA *compile(const std::string &regex);

    bool match(const std::string &regex, const std::string &input);
    bool match(const std::string &regex, const std::string &input, automate::groupMatcher& matcher);

    bool match(automate::DFA &compiled, const std::string &input);
    bool match(automate::DFA &compiled, const std::string &input, automate::groupMatcher& matcher);

    std::vector<std::string> findall(const std::string &regex, const std::string &input);
    std::vector<std::string> findall(const std::string &regex, const std::string &input, automate::groupMatcher& matcher);

    std::vector<std::string> findall(const automate::DFA &compiled, const std::string &input);
    std::vector<std::string> findall(const automate::DFA &compiled, const std::string &input, automate::groupMatcher& matcher);

    const automate::DFA *subtraction(automate::DFA &base, automate::DFA &subtrahend);
    const automate::DFA *subtraction(const std::string &base, const std::string &subtrahend);

    const automate::DFA *addition(const std::string &regex);
    const automate::DFA *addition(automate::DFA &compiled);

} 

#endif// MYREGEX_H
