#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <numeric>


int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string line{}; // Holds the line read from the file
    std::smatch match{}; // Holds matching groups in pattern 

    //Holds a frequency map for each position
    std::vector<std::unordered_map<char,  int>> positionFrequency(8); 

    // Reading file input line by line
    while(std::getline(file, line)) {
        std::regex pattern{"(\\w+)"}; // Pattern for extracting data
        if(std::regex_match(line, match, pattern)) {
            for(size_t i{0}; i < match[1].str().size(); i++) {
                positionFrequency[i][match[1].str()[i]]++; // Updating a letter's frequency in its associated position's map
            }
        }
    }

    // Strings that hold each answer
    std::string part1Answer{};
    std::string part2Answer{};

    for(const auto& map : positionFrequency) {
        auto it = map.begin();

        // Variables that will hold the least and most frequent letters and their associated frequency numbers
        char mostFrequentLetter{it->first};
        int maxFrequent{it->second};

        char leastFrequentLetter{it->first};
        int leastFrequent{it->second};
        

        for (++it; it != map.end(); ++it) {

            // Updating the most and least frequent letter if a letter with a greater or lower frequency, respectively, is found
            if (it->second > maxFrequent) {
                maxFrequent = it->second;
                mostFrequentLetter = it->first;
            }
            if(it->second < leastFrequent) {
                leastFrequent = it->second;
                leastFrequentLetter = it->first;
            }
        }

        // Building each part's answer
        part1Answer.push_back(mostFrequentLetter);
        part2Answer.push_back(leastFrequentLetter);
    }

    // Printing the answer
    std::cout << "Part 1: " << part1Answer << std::endl;
    std::cout << "Part 2: " << part2Answer << std::endl;

    return 0;


}
