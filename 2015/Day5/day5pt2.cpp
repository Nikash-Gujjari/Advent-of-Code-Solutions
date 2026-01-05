#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::unordered_map<std::string, int> pairs{}; // Stores list of found pairs and the index of the first 
    // character of the pair
    
    std::string pair; //Stores the current pair being looked at
    bool repeatedPairs{false}; // Checks if there is a current character
    std::string line; // Stores the current string being checked
    bool hasInBetween{false};  //Stores track of having a pair of equal characters with a char in between them
    int niceStrings{0}; // Keeps track of the number of nice stringss

    std::string prev{};
    std::string next{};

    // Reading in line by line
    while (std::getline(file, line)) {
        for(size_t i = 0; i + 1 < line.size(); i++) {
            pair = line.substr(i, 2);
            if(!repeatedPairs) {
                // Checks that a repeat pair is found and that the new pair does not overlap with the existing pair
                if (pairs.count(pair) > 0 && i - pairs[pair] > 1) {
                    repeatedPairs = true;
                } else {
                // Keeps track of pairs
                pairs.insert({pair, i});
                }
            }

            // Checks all 3 sets of characters using a previous and next character to the character in position i
            // Deems that there is a character in between a repeated character by checking if 'next' and 'prev' are
            // equal
            if(i > 0 && !hasInBetween) {
                prev = line[i-1];
                next = line[i+1];
                if(prev == next) {
                    hasInBetween = true;
                }

            }

        }

        // Nice Strings would satisfy the repeating pairs and in between repeated characters, character requirements
        if(repeatedPairs && hasInBetween) {
            niceStrings++;
        }

        pairs.clear();
        // Resetting values for next string
        repeatedPairs = false;
        hasInBetween = false;

    }
    file.close();
    std::cout << "Number of Nice Strings Found: " << niceStrings;

}