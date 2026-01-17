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

    
    int sideNum{1}; //Holds which side number of a triangle an input line is for
    std::vector<int> max; // Holds the largest side of each triangle
    std::vector<std::vector<int>> smaller; // Holds the 2 smallest sides of each triangle

    // Reading file input line by line
    while(std::getline(file, line)) {
        std::regex pattern{"\\s*(\\d+)\\s*(\\d+)\\s*(\\d+)\\s*"}; // Pattern for extracting data
        if(std::regex_match(line, match, pattern)) {
            // When starting a new triangle, max and smaller are reset to their default values
            if(sideNum == 1) {
                max = std::vector<int>{-1, -1, -1};
                smaller = std::vector<std::vector<int>>{std::vector<int>{}, std::vector<int>{}, std::vector<int>{}};
            }
            // For each triangle, find the max and two smaller sides
            for(size_t i{0}; i < match.size() - 1; i++) {
                int curr{0};
                if(max[i] == -1) { // If there is no max currently, our first value becomes the max
                    max[i] = std::stoi(match[i + 1].str());
                } else if((curr = std::stoi(match[i + 1].str())) > max[i]) { // If a new value is greater than max it becomes the max
                    // and the previous max is added to the smaller vector
                    smaller[i].push_back(max[i]);
                    max[i] = curr;
                } else { // If the current max is greater than the new value, the new value is added to the smaller vector
                    smaller[i].push_back(curr);
                }
            }

            if(sideNum == 3) { // If we have seen all 3 sides of each triangle, we can do the triangle inequality comparison to see
                // if the triangles are valid
                for(size_t i{0}; i < smaller.size(); i++) {
                    if(smaller[i][0] + smaller[i][1] > max[i]) {
                        triangleCount++;
                    }
                }
                sideNum = 1; // Resetting the sideNumber for the next set of triangles
            } else { // If not all 3 sides are seen, we continue to the next side number and their sizes
                sideNum++;
            }
        }
    }

    std::cout << "Number of Valid Triangles: " << triangleCount;

    return 0;


}
