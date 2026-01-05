#include <iostream>
#include <string>

// Checks if a String is valid
bool checkString(std::string input) {


    // Checking for invalid characters, first
    if(input.find('i') != std::string::npos || input.find('o') != std::string::npos || input.find('l') != std::string::npos) {
        return false;
    }

    char prev = input[0]; // Holds the previous character

    int repeatPairCount{0}; // Counts the number of distinct, non-overlapping repeat pairs

    bool overlapCheck{false}; // Used to make sure repeat pairs do not overlap. When the second character of a repeat pair is found 
    // this variable is set to true for the next character, then returns to false until another repeat pair is found

    bool incrementPair{false}; // Keeps track of a found incrementing pair, i.e. 'ab', 'xy', etc.

    bool incrementTriple{false}; // Keeps track of a found incrementing triple, i.e. 'abc', 'efg', etc.

    // Looping through all characters
    for(size_t i{1}; i < input.size(); i++) {

        // Checking for a repeat pair that does NOT overlap with a previously found repeat pair, through
        // the use of the overlapCheck variable
        if(prev == input[i] && !overlapCheck) {
            repeatPairCount++;
            overlapCheck = true;
        } else {
            // Returns overlapCheck to false
            overlapCheck = false;
        }

        // Checking for incrementing sequence
        if(input[i] == prev + 1) {
            // If incrementPair is true, then another incrementing character means that there is an increment triple present.
            // Otherwise, if incrementPair is false, then an incrementing character means there is only an increment pair present.
            if(incrementPair) {
                incrementTriple = true;
            } else {
                incrementPair = true; 
            }
        } else {
            incrementPair = false; // This resets incrementPair when there is not an increment triple eventually present, after finding
            // two adjacent characters in an incrementPair
        }

        // Exiting loop early if conditions are met
        if(incrementTriple && repeatPairCount >= 2) {
            return true;
        }

        prev = input[i]; // Updating the previous


    }

    return false;
}

void increment(std::string& input, int index){

    if(input[index] == 'z') {
        input[index] = 'a';
        if(index - 1 == -1) {
            input = 'a' + input;
        } else {
            increment(input, index-1);
        }
    } else {
        input[index]++;
    }
}
int main() {
    std::string input = "cqjxjnds";

    while(!checkString(input)) {
        increment(input, input.size()-1);
    }
    
    std::cout << "Next Password: " << input << '\n';

    // Part 2
    increment(input, input.size()-1);

    while(!checkString(input)) {
        increment(input, input.size()-1);
    }


    std::cout << "Password After Next: " << input;

    return 0;

}