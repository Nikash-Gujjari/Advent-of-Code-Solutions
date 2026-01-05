#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main() {

    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    // Read the file into a stringstream
    std::stringstream buffer;
    buffer << file.rdbuf();  // rdbuf() gets the file's internal buffer

    // Convert the stringstream to a string
    std::string fileContent = buffer.str();


    int floor{0}; // Floor Santa starts on (0)
    int position{0}; // Position of the characters 
    bool foundFirst{false}; // For Part 2, keep track of which character causes him to enter the basement


    // Iterating through all characters and updating Santa's position accordingly
    for(char & c : fileContent) {
        position++;

        if(c == '(') {
           floor++; 
        }

        if(c == ')') {
            floor--;
        }

        if(floor == -1 && !foundFirst) {
            std::cout << "Character position that first causes Santa to enter basement (-1): " << position << "\n"; // Printing which 
            // character position first causes Santa to enter the basement
            foundFirst = true;
        }
    }

    std::cout << "What floor Santa Ends Up On: " << floor;
    file.close();  // Close the file

    return 0;

}