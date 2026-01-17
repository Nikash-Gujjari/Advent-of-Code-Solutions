#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string line{};
    std::smatch match{}; // Holds matching groups in pattern 

    // Reading file input line by line
    while(std::getline(file, line)) {
        
    }

    return 0;


}
