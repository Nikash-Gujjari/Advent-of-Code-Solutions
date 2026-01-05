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
    
    std::smatch match{}; // Holds matching groups in pattern 
    std::string line{}; // Holds the line read from the file

    std::vector<int> container_sizes{}; // Holds container sizes from input

    // Reading in line by line
    while (std::getline(file, line)) { 
       container_sizes.push_back(std::stoi(line));
    }

    file.close();

    const int TARGET{150}; // How many Liters need to be filled 

    std::vector<int> dp(TARGET + 1, 0); // dp[i] = the number of contianer combinations that hold exaclty i Liters

    dp[0] = 1; // Base case. For 0 Liters, there is only the empty set of containers that exactly hold it. 


    // Uses dynamic programming to count combinations using a top-down 0/1 Knapsack algorithm, which prevents reuse of containers. 
    for(const int& container_type : container_sizes) {

        for(size_t i{TARGET}; i >= container_type; i--) {    
            dp[i] += dp[i - container_type];
            
        }

    }


    std::cout << "Number of combinations: " << dp[TARGET] << std::endl;


    // Part 2


    // Using dynamic programming again to get the minimum number of containers to hold 150 L, and the number of combinations
    // that hold it
    // dpPT2[i][j] = the number of combinations that hold i Liters with j number of containers. This splits the number of ways
    // to reach i Liters by the number of containers. 
    std::vector<std::vector<int>> dpPT2(
        TARGET + 1,
        std::vector<int>(container_sizes.size() + 1, 0)
    );


    dpPT2[0][0] = 1; // Starting value represents the empty set (size 0 set) can hold 0 Liters

    // Using dynamic programming again to count combinations using (top-down) 0/1 Knapsack algorithm
    for(const int& container_type : container_sizes) {

        for(size_t i{TARGET}; i >= container_type; i--) {    
        
            for(size_t j{1}; j < container_sizes.size(); j++) {
                // Adding a container increases previous set's volume size and the set size (number of containers) by 1, so we use
                // [i - container_type] for previous set's volume size and [j - 1] for the previous set's size
                dpPT2[i][j] += dpPT2[i - container_type][j - 1];
            }

        }

    }

    // After populating the matrix, we can look at the min(dpPT2[TARGET][j]) for all possible j to find a 
    // j* that represents the minimum number of containers to reach TARGET liters. The number of combinations that use the minimum
    // number of containers that reach TARGET liters would then be = dpPT2[TARGET][j*] 

    int min_containers_needed{INT_MAX}; 

    for(size_t j{1}; j < container_sizes.size(); j++) {

        // 0 is the default value we populated at creation of the matrix, and so we ignore that j
        if(dpPT2[TARGET][j] != 0) {
            // Since we go from lowest to highest j's, the first non-zero dpPT2[TARGET][j] would have j = j*
            min_containers_needed = j;
            break;
        }

    }



    std::cout << "Minimum number of containers needed: " << min_containers_needed << std::endl;
    std::cout << "Number of combinations for this minimum number of containers: " << dpPT2[TARGET][min_containers_needed] << std::endl;


    return 0;


}