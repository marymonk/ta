#include <iostream>
#include <string>

#include <myRegex.h>

int main() {
    auto f = myRegex::findall("a*(z|s)c{3}", "aaasccc");
    for (auto &i : f) {
        std::cout << i <<std::endl;
    }
    return 0;
}
