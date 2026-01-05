#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


// Finds all permutations of a vector of strings
void permute(std::vector<std::string>& v, size_t pos, std::vector<std::vector<std::string>>& result) {

    // Represents only element left, so there is no other choice
    if (pos == v.size() - 1) {
        result.push_back(v);
        return;
    }

    std::string temp{};

    // Swapping the element at [pos] with [i] to calculate permutations. Swap is used to save memmory 
    // when backtracking for other 
    for (size_t i{pos}; i < v.size(); i++) {

        // fix (choosing) an element at the current position
        temp = v[pos];
        v[pos] = v[i];
        v[i] = temp;   

        // recurse to choose the next element
        permute(v, pos + 1, result); 

        //undo choice and backtrack for other results
        temp = v[pos];
        v[pos] = v[i];
        v[i] = temp;   
        
    }
}

// Algorithm
// Parse each line into happiness[A][B]
// Fix one guest and generate all permutations of the remaining guests
// For each circular arrangement, sum h[A][B] + h[B][A] for each adjacent pair (wrapping around)
// Keep track of the maximum total happiness
int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

        std::regex pattern{
        "(\\w+) would (gain|lose) (\\d+) happiness units by sitting next to (\\w+)."
    };
    std::smatch match; // Holds matching groups in pattern 
    std::string line{}; // Holds the line read from the file

    std::unordered_set<std::string> names{}; // Holds the set of names

    std::unordered_map<std::string, std::unordered_map<std::string, int>> happiness{}; // keeps track of happiness, such
    // that happiness[A][B] represents how much happiness A would gain/lose from sitting next to B

    // Reading in line by line
    while (std::getline(file, line)) {

        // Storing data into the happiness data structure
        if (std::regex_search(line, match, pattern)) {
            names.insert(match[1].str());
            happiness[match[1].str()][match[4].str()] = ((match[2].str() == "gain") ? 1 : -1) * std::stoi(match[3].str());
        }       
    }

    file.close();

    // Part 2: Adding yourself and giving all happiness relationships that involve you a score of 0
    for(const auto& name : names) {
        happiness["Me"][name] = 0;
        happiness[name]["Me"] = 0;
    }
    names.insert("Me");

    
    std::vector<std::string> starting_permutations(names.begin(), names.end()); //Copying the set of names into a vector

    std::string fixed{starting_permutations[starting_permutations.size() - 1]}; // Fixing one name at the end (in between first and last
    // person of the other permutations made)
    starting_permutations.pop_back();


    std::vector<std::vector<std::string>> all_permutations_except_fixed{};
    permute(starting_permutations, 0, all_permutations_except_fixed); // Storing all created permutations in a vector of vector strings
 
    int max_happiness{0}; // Stores the highest happiness found
    int current_happiness{0}; // Stores the hapiness of the current permutation as calculated

    for (const auto& p : all_permutations_except_fixed) {

        // Calculating for all permutations except the fixed
        for (size_t i{0}; i < p.size() - 1; i++) {
            current_happiness += happiness[p[i]][p[i+1]] + happiness[p[i+1]][p[i]];
        }

        // Adding the fixed person's relationship with the first and last person (they are fixed in between these two people)
        current_happiness += happiness[p[p.size()-1]][fixed] + happiness[fixed][p[p.size() - 1]];
        current_happiness += happiness[p[0]][fixed] + happiness[fixed][p[0]];


        // Updating max_happiness and reseting current_happiness for next iteration
        max_happiness = std::max(max_happiness, current_happiness);
        current_happiness = 0;
    }

    std::cout << "Greatest Happiness Possible: " << max_happiness;

    return 0;

}