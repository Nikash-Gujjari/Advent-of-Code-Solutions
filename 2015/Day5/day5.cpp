#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>

int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::unordered_set<char> vowelSet{'a', 'e', 'i', 'o', 'u'}; // Stores set of vowels
    std::unordered_set<std::string> badPairs{"ab", "cd", "pq", "xy"}; // Stores list of naughty pairs
    
    char prev{'\0'}; // Stores the previous character. Specifically set to this value to be falsey
    char curr{'\0'}; // Stores the current character
    bool repeated{false}; // Checks if there is a current character
    std::string line; // Stores the current string being checked
    bool hasBadPair{false}; 
    int niceStrings{0}; // Keeps track of the number of nice stringss
    std::string prevAndCurr{}; // String to store the pair of the previous char + current char
    int vowelCount{0};

    // Reading in line by line
    while (std::getline(file, line)) {
        for(size_t i{0}; i < line.size(); i++) {
            curr = line[i];
            // Removing vowels from vowelSet when they are found
            if(vowelSet.find(curr) != vowelSet.end()) {
                vowelCount++;
            }
            if(prev) {

                // Checking for a repetition
                if(!repeated) {
                    repeated = (prev == curr);
                }
                prevAndCurr = std::string(1, prev) + curr;

                // Checking for bad pairs
                if(!hasBadPair && badPairs.find(prevAndCurr) != badPairs.end()) {
                    hasBadPair = true;
                    break;
                }       
            }

            prev = curr; // Updating prev
        }

        // Nice Strings would remove at least 3 vowels from vowelSet(so it would have <= 2 remaining),
        // have a repeated character, and not have a bad pair
        if(vowelCount >= 3 && repeated && !hasBadPair) {
            niceStrings++;
        }

        // Resetting values for next string
        vowelCount = 0;
        prev = '\0';
        repeated = false;
        hasBadPair = false;

    }
    file.close();
    std::cout << "Number of Nice Strings Found: " << niceStrings;

    return 0;

}