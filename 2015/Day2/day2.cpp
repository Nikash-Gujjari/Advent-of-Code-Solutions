#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

// Function that splits a string by a delimiter and returns each piecie in a vector
std::vector<int> split(const std::string& str, char delimiter) {
    std::vector<int> tokens;
    std::string token;
    std::stringstream ss(str);

    // Split the string using the delimiter
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(std::stoi(token));
    }

    return tokens;
}

int main() {
    std::ifstream file("input.txt");
    int totalWrappingPaper{0};
    int totalRibbon{0};
    if (file.is_open()) {
        std::string line;
        // Reading in line by line
        while (std::getline(file, line)) {
            
            std::vector<int> dimensions = split(line.c_str(), 'x'); // Get dimensions as int vector
            std::sort(dimensions.begin(), dimensions.end()); // Sort the int vector in default ascending order
            int slack{dimensions[0]*dimensions[1]}; // Get slack 
            int surfaceArea{2*dimensions[0]*dimensions[1] + 2*dimensions[1]*dimensions[2] + 2*dimensions[0]*dimensions[2]};
            int wrapRibbon{2*dimensions[0] + 2*dimensions[1]};
            int bowRibbon{dimensions[0]*dimensions[1]*dimensions[2]};
            totalWrappingPaper += slack + surfaceArea;
            totalRibbon += wrapRibbon + bowRibbon;
        }
        file.close();
        std::cout << "Total Wrapping Paper For Elves: " << totalWrappingPaper << "\n";
        std::cout << "Total Ribbon For Elves: " << totalRibbon;
        return 0;
    } else {
        std::cout << "File not opened";
        return 1;
    }

}
