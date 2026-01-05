#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>


int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string line{}; // Holds the line read from the file
    int inMemoryCharDiff{0}; // Will hold the count of characters in memeory

    int newEncodingDiff{0}; // Part 2


    // Reading in line by line
    while (std::getline(file, line)) {

        inMemoryCharDiff += 2; // Adding Leading and trailing "". 
        
        
        // Loop ignores ending quote

        size_t i{1}; 

        while (i < line.size()) {

            // Checking for escaped sequences
            if (line[i] == '\\') {

                // Escaped '\' or Escaped '\'
                if (line[i + 1] == '\"' || line[i + 1] == '\\') {
                    inMemoryCharDiff += 1;     // code 2 chars, memory 1 char
                    i += 2;
                    continue;
                }

                // Escaped hex sequences
                if (line[i + 1] == 'x') {
                    inMemoryCharDiff += 3;     // code 4 chars, memory 1 char
                    i += 4;
                    continue;
                }
            }

            // normal character
            i += 1;
        }

        // Part 2

        i = 0;

        while (i < line.size()) {

            // Starting and Ending '"'
            if(line[i] == '\"' || line[i] == '\\') {
                newEncodingDiff++; // Escaping '"' and '\\' for the encoding
            }
            i++;
        }

        newEncodingDiff+= 2; // Extra Leading and Traing '"'s

    }
    file.close();

    std::cout << "Memory Difference: " << inMemoryCharDiff << "\n";
    std::cout << "[Part 2] Encoding Difference: " << newEncodingDiff; 

    return 0;


}