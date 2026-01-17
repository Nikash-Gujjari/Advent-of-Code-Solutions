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

    std::string line{}; // Holds the line read from the file
    std::smatch match{}; // Holds matching groups in pattern 

    int triangleCount{0}; // Holds the number of valid triangles found

    // Reading file input line by line
    while(std::getline(file, line)) {
        std::regex pattern{"\\s*(\\d+)\\s*(\\d+)\\s*(\\d+)\\s*"}; // Pattern for extracting data
        std::vector<int> smaller{}; // Holds the 2 smaller side values of a triangle
        int max{-1}; // Hold the largest side length
        if(std::regex_match(line, match, pattern)) {
            for(size_t i{1}; i < match.size(); i++) {
                int curr{0};
                if(max == -1) { 
                    max = std::stoi(match[i].str()); 
                } else if((curr = std::stoi(match[i].str())) > max) { // If a new value is greater than max it becomes the max
                    // and the previous max is added to the smaller vector
                    smaller.push_back(max);
                    max = curr;
                } else { // If the current max is greater than the new value, the new value is added to the smaller vector
                    smaller.push_back(curr);
                }
            }
            if(smaller[0] + smaller[1] > max) { // Checking if the smaller sides are greater than the largest side. If so, this is a valid triangle
                triangleCount++;
            }
        }
    }

    std::cout << "Number of Valid Triangles: " << triangleCount;

    return 0;


}
