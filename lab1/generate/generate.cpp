#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

std::string generateRandomString(int length) {
    std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
    std::string result = "";

    for (int i = 0; i < length; i++) {
        result += characters[rand() % characters.size()];
    }

    return result;
}

void generateRandomPairs(std::ofstream& outFile, int numPairs) {
    for (int i = 0; i < numPairs; i++) {
        std::string parentClass = generateRandomString(8);
        std::string childClass = generateRandomString(8);
        std::string functionName = generateRandomString(8) + "()";

        outFile << "class " << parentClass << ": " << "private " << childClass << " { " << functionName << " };" << std::endl;
    }
}

int main() {
    srand(time(NULL));

    int numPairs = 100;

    std::ofstream outFile("D:/inf/sem4/ta/generate/generated_file.txt");
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    generateRandomPairs(outFile, numPairs);

    outFile.close();

    std::cout << "Generated code has been saved to generated_code.txt" << std::endl;

    return 0;
}