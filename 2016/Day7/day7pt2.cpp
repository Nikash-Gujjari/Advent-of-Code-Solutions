#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_set>

// Finds an ABA sequence in a supernet sequence (non-hypernet) and then finds the corresponding BAB sequence in
// a hypernet sequence
bool isThereABAandBABsequence(const std::vector<std::string>& superNets, const std::vector<std::string>& hyperNets) {
    for(const std::string& superNetSequence : superNets) {
        for(size_t pos{0}; pos + 2 < std::size(superNetSequence); pos++) {
            // Finding a ABA sequence
            if((superNetSequence[pos] == superNetSequence[pos + 2]) && superNetSequence[pos] != superNetSequence[pos + 1]) {
                // Creating its corresponding BAB sequence
                std::string bab{superNetSequence.substr(pos + 1, 1) + superNetSequence.substr(pos, 1) + superNetSequence.substr(pos + 1, 1)};
                // Looking for that BAB sequence in a hypernet sequence
                for(const std::string& hyperNetSequence : hyperNets) {
                    if(hyperNetSequence.find(bab) != std::string::npos) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string line{}; // Holds the line read from the file
    std::smatch match{}; // Holds matching groups in pattern 

    int supportSSL{0};

    // Reading file input line by line
    while(std::getline(file, line)) {
        std::size_t startSearchIndex{0}; // The index from where the find function is ran
        std::size_t closeBracketIndex{0}; // The index of the first close Bracket after the start search index

        std::vector<std::string> superNets{}; // Vectors that hold all of the supernet sequences
        std::vector<std::string> hyperNets{}; // Vector that holds all of the hypernet sequences
        while((closeBracketIndex = line.find("]", startSearchIndex)) !=  std::string::npos) { // Iterating
            // through the string and finding the next hypernet and non-hypernet sequences. Search terminates early if an ABBA pattern
            // is found in a hypernet sequence. 
            std::size_t openBracketIndex = line.find("[", startSearchIndex); // Finding the openBracket index of hypernet sequence
            // Using indexes of first found open and closed Brackets from starting search index to 
            // extract hypernet and non-hypernet sequences and adding them to the appropriate vector
            superNets.push_back(line.substr(startSearchIndex, openBracketIndex - startSearchIndex)); 
            hyperNets.push_back(line.substr(openBracketIndex + 1, closeBracketIndex - openBracketIndex - 1));
            // Checking for ABBA patterns in hypernet and non-hypernet sequences
            startSearchIndex = closeBracketIndex + 1; // Making the next search start on the index right after the close bracket
        }
        superNets.push_back(line.substr(startSearchIndex)); // Adding the last supernet sequence
        supportSSL += (isThereABAandBABsequence(superNets, hyperNets)) ? 1 : 0; // Incrementing supportTSSL counter if the conditions are met
    }

    std::cout << "Number of IPs that support SSL: " << supportSSL;


    return 0;


}
