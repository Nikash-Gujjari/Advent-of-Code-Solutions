#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>

int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::unordered_map<std::string, int> knownSue{
        {"children", 3},
        {"cats", 7},
        {"samoyeds", 2},
        {"pomeranians", 3},
        {"akitas", 0}, 
        {"vizslas", 0},
        {"goldfish", 5},
        {"trees", 3},
        {"cars", 2},
        {"perfumes", 1}
    };
    
    std::smatch match{}; // Holds matching groups in pattern 
    std::string line{}; // Holds the line read from the file

    int currentSueNumber{}; // Keeps track of the number of current Sue from input line

    // Reading in line by line
    while (std::getline(file, line)) { 

        std::regex pattern{"Sue (\\d+):"}; // Pattern for extracting Sue number

        std::regex re("(\\w+):\\s*([^,]+)"); // Pattern for getting all of a Sue's details

        std::unordered_map<std::string, int> currentSue{}; // Keeps track of current Sue properties
       

        // Finding the Sue Number
        if (std::regex_search(line, match, pattern)) {
            currentSueNumber = std::stoi(match[1].str());
        }       

        line = line.substr(line.find_first_of(":")); // Removing the Sue (number): 

        // Use sregex_iterator to find all matches
        auto begin = std::sregex_iterator(line.begin(), line.end(), re);
        auto end = std::sregex_iterator();
        for (auto it = begin; it != end; ++it) {
            std::smatch match = *it;
            std::string field = match[1].str();
            int value = std::stoi(match[2].str());

            currentSue[field] = value;
        }

        bool foundSue{true}; // Keeps track of whether a currentSue in the loop
        // below is found to have a disagreeable property when the loop is finished
        // (through a break statement [disagreeable property] or end of iteration [found a correct Sue])

        // Looping through the currentSue's properties and attempting to find
        // a property that does equal the knownSue's properties from the MFCSAM scan
        for(const auto& [property, value]: currentSue) {

            // Part 2: Checking that cats & trees are greater than the MFCSAM's results, 
            // and pomeranians & goldfish are less than. 
            if((property == "cats" || property == "trees")) {
                if(value <= knownSue[property]) {
                    foundSue = false;
                    break;
                }
            } else if(property == "pomeranians" || property == "goldfish") {
                if(value >= knownSue[property]) {
                    foundSue = false;
                    break;
                }
            } else if(knownSue[property] != value) { // Breaking out of the loop 
                // if a property does not match and seeting foundSue to false
                // to signal that there was a disagreeing property
                foundSue = false;
                break;
            }

        }

        // Using the foundSue variable, if there were no disagreeing property then
        // the currentSue is the foundSue
        if(foundSue){
            break;
        }


    }

    file.close();

    std::cout << "The correct Sue #: " << currentSueNumber;

    return 0;



}