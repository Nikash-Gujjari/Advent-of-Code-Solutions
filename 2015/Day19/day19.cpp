#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>


int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }
    
    std::smatch match{}; // Holds matching groups in pattern 
    std::string line{}; // Holds the line read from the file

    bool blankLine{false};

    std::string medicine_molecule{}; // Holds the initial medicine molecule at the end of the file

    std::unordered_map<std::string, std::vector<std::string>> replacements{}; // A map to hold string replacements


    // Reading in line by line
    while (std::getline(file, line)) { 

        // Processing the blank line and medinice molecule at the end of the input file
        // Once a blank line is reached the next input is the medicine molecule
        if(line.size() == 0) {
            blankLine = true;
            continue;
        }
        if(blankLine) {
            medicine_molecule = line;
            break;
        }

        // Processing all replacement information into the replacements map
        std::regex pattern{"(\\w+) => (\\w+)"}; // Pattern for extracting string conversions
        if (std::regex_search(line, match, pattern)) {
            replacements[match[1].str()].push_back(match[2].str());
        }   

    }
    
    file.close();

    std::unordered_set<std::string> created_molecules{}; // Holds unique molecules created after replacement

    for(size_t pos{0}; pos < medicine_molecule.size(); pos++) {
        for(const auto& [key, new_strings]: replacements) {
            // Checking if it is possible for a replacement
            if(medicine_molecule.substr(pos, int(key.size())) == key) {
                // Create a string replacement and add it to the set.
                // The set ensures that only unique strings are within the set. 
                for(const std::string& string: new_strings) {
                    std::string curr_string = medicine_molecule;
                    curr_string.replace(pos, key.size(), string);
                    created_molecules.insert(curr_string);
                }
            }
        }
    }


    // The set has all the distinct, created molecules. The size of it
    // gives the number of distinct, created molecules. 

    std::cout << "Distinct molecules created: " << created_molecules.size() << std::endl;

    // Part 2

    // Assuming the molecules replacement as context-free grammar rules from which the medicine molecule is validly derived, we can see some important structural nuances:
    // If we see: 
    // Rn → “opening parenthesis”
    // Ar → “closing parenthesis”
    // Y → “comma” / separator between multiple children 
    
    // THEN:

    // 1. Every replacement produces exactly one “parent node” with some number of children.
    // 2. No rule produces multiple top-level nodes at once, so every internal node corresponds to exactly one replacement step. Parentheses and comma molecules (Rn, Ar, Y) only define hierarchy and they only group children under a parent.
    // 3. The medicine molecule is a single connected tree: no unconnected tokens; everything ultimately descends from e; no ambiguity in how many steps are needed to assemble children under a parent.
    // 4. Leaves are always terminal symbols.


    // We will let the leaves be the given medicine molecule and our root be e. 

    // Then, by using a parse tree representation, we know that the number of replacement steps is the number of internal nodes from the root (e) to our full molecule (medicine_molecule), which is transitioned via the grammar rules. 

    // However, we do not consider Rn, Ar, and Y because of their roles as parantheses/commas. Thus, we should use an AST representation that only has internal nodes that are non-terminal by our grammar rulse.

    // In the AST representation, the number of internal nodes = (total # of molecules in medicine molecules) - (# of Rn) - (# of Ar) - 2 * (# of Y) - 1. We need to remove 2 for every Y because each Y seperates two sibilings, and so in replacement each Y is itself a token and saves an aditional token (the second sibiling). The '-1' is needed so that we do not double count the root 'e' as an internal node on top of it being a given root node. 

    // Splitting the medicine molecules into nodes for calculation
    std::vector<std::string> split_medicine_molecule{};

    for(size_t pos{0}; pos < medicine_molecule.size(); pos++) {
        if(pos < medicine_molecule.size() - 1 && islower(medicine_molecule[pos + 1])) {
            split_medicine_molecule.push_back(medicine_molecule.substr(pos, 2));
            pos++;
        } else {
            split_medicine_molecule.push_back(medicine_molecule.substr(pos, 1));
        }

    }

    // Finding information for the formula:
    // replacement steps = (total # of molecules in medicine molecules) - (# of Rn) - (# of Ar) - 2 * (# of Y) - 1
    int total_tokens{int(split_medicine_molecule.size())}; 
    int rn_token{0};
    int ar_token{0};
    int y_token{0};
    for(const std::string& molecule: split_medicine_molecule) {
        if(molecule == "Y") {
            y_token++;
        } else if(molecule == "Rn") {
            rn_token++;
        } else if(molecule == "Ar") {
            ar_token++;
        }
    }

    // Formula calculation and printing
    std::cout << "Number of steps: " << total_tokens - rn_token - ar_token - 2 * y_token - 1;

    return 0;

}