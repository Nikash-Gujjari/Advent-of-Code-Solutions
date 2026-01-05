#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

// Returns an int vectors of all factors of a number
std::vector<int> getFactors(const int num) {
    std::vector<int> factors{};

    // We only need to look up integers until sqrt(N) 
    for(int i{1}; i * i <= num; i++) {
        if(num % i == 0) {
            factors.push_back(i);
            if(num / i != i) {
                factors.push_back(num / i);
            }
            
        }
    }
    return factors;
}

// Returns an int vectors of all factors of a number that are
// less than 50 repetitions from the number
std::vector<int> getFilteredFactors(const int num) {
    std::vector<int> factors{};

    // We only need to look up integers until sqrt(N) 
    for(int i{1}; i * i <= num; i++) {
        if(num % i == 0) {
            // The two factors of num are i and (num / i). Since we need 50 * i >= num by
            // our filtering condition, we end up just checking that the complement factor
            // of one of the factors is <= 50 so that the factor * 50 >= num. 
            // i.e. Checking i <= 50 is equivalent to checking if  num / [(num / i)] <= 50, which
            // would be the calculation behind checking if the second factor * 50 >= num.  
            if(num / i <= 50) {
                factors.push_back(i);
            }
            if(num / i != i && i <= 50) {
                factors.push_back(num / i);
            }
            
        }
    }
    return factors;
}


int main() {

    const int INPUT{36000000};

    int presentSum{0}; // Holds the number of presents delivered
    int houseNumber{0}; // Holds House number. House number is incremented before factors are found so the algorithm effectively starts
    // at House number 1, even if it is declared as 0 here. 

    // Factors are found, which represent the elf number that visits. Each is multiplied by 10 since
    // each elf delivers 10 presents. The calculation is done until a sum that is >= INPUT is found
    while(presentSum < INPUT) {
        presentSum = 0;
        houseNumber++; 
        std::vector<int> factors = getFactors(houseNumber);
        for(const int& factor : factors) {
            presentSum += factor * 10;
        }
    }

    // Printing Part 1 Solution
    std::cout << "Lowest house number to get at least " << INPUT << " presents: " << houseNumber << std::endl;

    // Part 2

    int part2PresentSum{0}; // Holds the number of presents delivered for Part 2
    int part2HouseNumber{0}; // Holds House number for part 2. House number is incremented before factors are found so the algorithm effectively starts
    // at House number 1, even if it is declared as 0 here. 

    // Factors are found for which the factor * 50 >= house_number, since elves [factor] only visit 50 houses. getFilteredFactors filters factors
    // based on this condition. 
    while(part2PresentSum < INPUT) {
        part2PresentSum = 0;
        part2HouseNumber++;
        std::vector<int> filteredFactors = getFilteredFactors(part2HouseNumber);
        for(const int& factor : filteredFactors) {
            part2PresentSum += factor * 11;
        }
                
    }
    
    // Printing Part 2 Solution
    std::cout << "PART 2: Lowest house number to get at least " << INPUT << " presents: " << part2HouseNumber << std::endl;

    return 0;

}