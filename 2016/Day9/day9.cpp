#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

// Gets all the contents of a file (except whitespace) into a single string
std::string fileToStringNoWhitespace(const std::string& filename)
{
    std::ifstream file(filename);
    std::string result;
    char ch;

    while (file.get(ch)) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            result += ch;
        }
    }

    return result;
}


// Part 2 in which subsequences that include markers are recursively expanded as well
size_t decompressRecursion(std::string substr) {
    size_t currPos{0}; // Holds the current position of the input text
    size_t decompressedLength{0}; // The total decompressed length calculated
    size_t openMarker{0}; // Holds the position of the open parenthesis of a marker

    while((openMarker = substr.find('(', currPos)) != std::string::npos) { // Seeing if there is a marker and if so
        // setting openMarker's value

        size_t xOfMarker = substr.find('x', currPos); // Location of the 'x' in the marker
        size_t closedMarker = substr.find(')', currPos); // Location of the closed parenthesis
        
        // Extracting data from the Marker
        int numChars{std::stoi(substr.substr(openMarker + 1, xOfMarker - openMarker - 1))}; // Number of characters described by the marker
        int repetition{std::stoi(substr.substr(xOfMarker + 1, closedMarker - xOfMarker - 1))}; // The repition value described by the marker

        decompressedLength += openMarker - currPos;  // Adding the decompression length of the number of characters that are not influenced by the marker
        decompressedLength += decompressRecursion(substr.substr(closedMarker + 1, numChars)) * repetition; // For part 2, the sequence of characters influenced by the marker are recursively expanded through this recursion instead of ignored like in par 1. The sequence's length is then subject to the repition factor described by the original marker

        currPos = closedMarker + 1 + size_t(numChars); // Setting the position to the index after the last character that is influenced by the marker
    }

    decompressedLength += substr.size() - currPos; // Adding the final non-marker sequence length 

    return decompressedLength;
}

int main() {


    std::string fileInput{fileToStringNoWhitespace("input.txt")}; // Getting all contents of file, except for the whitespace

    size_t currPos{0}; // Holds the current position of the input text
    size_t decompressedLength{0}; // The total decompressed length calculated
    size_t openMarker{0}; // Holds the position of the open parenthesis of a marker

    while((openMarker = fileInput.find('(', currPos)) != std::string::npos) { // Seeing if there is a marker and if so
        // setting openMarker's value

        // If there is an open parenthesis, we can expect to find the 'x' and a closed parenthesis
        size_t xOfMarker = fileInput.find('x', currPos); // Location of the 'x' in the marker
        size_t closedMarker = fileInput.find(')', currPos); // Location of the closed parenthesis

        // Extracting data from the Marker
        int numChars{std::stoi(fileInput.substr(openMarker + 1, xOfMarker - openMarker - 1))}; // Number of characters described by the marker
        int repetition{std::stoi(fileInput.substr(xOfMarker + 1, closedMarker - xOfMarker - 1))}; // The repition value described by the marker

        decompressedLength += openMarker - currPos; // Adding the decompression length of the number of characters that are not influenced by the marker
        decompressedLength += numChars * repetition; // Adding the decompression length of the number of characters that are influenced by the marker and are therefore have their contribution multiplied by the repition value

        currPos = closedMarker + 1 + size_t(numChars); // Setting the position to the index after the last character that is influenced by the marker
    }

    decompressedLength += fileInput.size() - currPos; // Adding the final non-marker sequence length 

    // Printing output
    std::cout << "Decompressed Length: " << decompressedLength << std::endl;
    std::cout << "[Part 2] Decompressed Length: " << decompressRecursion(fileInput);

    return 0;
}

