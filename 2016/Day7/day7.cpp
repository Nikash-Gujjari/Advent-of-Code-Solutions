#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_set>

// Returns true if there is a 4 letter character sequence that consists of a pair of two different characters 
// followed by the reverse of that pair, such as xyyx or abba. Returns false otherwise. 
bool findABBAsequence(const std::string& sequence) {
    for(size_t pos{0}; pos + 3 < std::size(sequence); pos++) {
        if((sequence[pos] == sequence[pos + 3]) && (sequence[pos + 1] == sequence[pos + 2]) && sequence[pos] != sequence[pos + 1]) {
            return true;
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

    int supportTLS{0};

    // Reading file input line by line
    while(std::getline(file, line)) {
        std::size_t startSearchIndex{0}; // The index from where the find function is ran
        std::size_t closeBracketIndex{0}; // The index of the first close Bracket after the start search index
        bool ABBAInHypernet{false};
        bool ABBAInNonHypernet{false};
        while((closeBracketIndex = line.find("]", startSearchIndex)) !=  std::string::npos && !ABBAInHypernet) { // Iterating
            // through the string and finding the next hypernet and non-hypernet sequences. Search terminates early if an ABBA pattern
            // is found in a hypernet sequence. 
            std::size_t openBracketIndex = line.find("[", startSearchIndex); // Finding the openBracket index of hypernet sequence
            // Using indexes of first found open and closed Brackets from starting search index to 
            // extract hypernet and non-hypernet sequences
            std::string nonHypernet = line.substr(startSearchIndex, openBracketIndex - startSearchIndex); 
            std::string hypernet = line.substr(openBracketIndex + 1, closeBracketIndex - openBracketIndex - 1);
            // Checking for ABBA patterns in hypernet and non-hypernet sequences
            ABBAInHypernet = findABBAsequence(hypernet) || ABBAInHypernet; 
            ABBAInNonHypernet = findABBAsequence(nonHypernet) || ABBAInNonHypernet;
            startSearchIndex = closeBracketIndex + 1; // Making the next search start on the index right after the close bracket
        }
        if(!ABBAInNonHypernet) { // Checking the last non-hypernet sequence, if needed, for the ABBA pattern
            ABBAInNonHypernet = findABBAsequence(line.substr(startSearchIndex)) || ABBAInNonHypernet; 
        }

        supportTLS += (!ABBAInHypernet && ABBAInNonHypernet) ? 1 : 0; // Incrementing supportTLS counter if conditions are met
    }

    std::cout << "Number of IPs that support TLS: " << supportTLS;


    return 0;


}
