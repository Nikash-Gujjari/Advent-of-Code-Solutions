#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

// Custom hash function for std::vector<int>
struct VectorHash {
    template <typename T>
    size_t operator ()(const std::vector<T>& vec) const {
        size_t hashValue = 0;
        for (const auto& elem : vec) {
            hashValue ^= std::hash<T>{}(elem) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
        }
        return hashValue;
    }
};

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
    
    // Set to keep track of unique locations where Santa and RoboSanta have gone
    std::unordered_set<std::vector<int>, VectorHash> set{};

    // Using an x,y coordinate system to track where Santa goes
    std::vector<int> santaPosition{0, 0};

    // Using an x,y coordinate system to track where Santa goes
    std::vector<int> roboSantaPosition{0, 0};

    // Adding the initial position (0, 0), since both Santas always deliver to their shared starting location
    set.insert(santaPosition);

    // Keeps track of Santa or RoboSanta by swapping boolean values
    bool isSanta = true;

    // Keeps track of whichever Santa is being directed, position
    std::vector<int>* position;

    for(char & c : fileContent) {

        position = (isSanta) ? &santaPosition : &roboSantaPosition;

        // Maintaing position based on found character
        if(c == '>') {
            (*position)[0]++;
        } else if (c == '<') {
            (*position)[0]--;
        } else if (c == '^') {
            (*position)[1]++;
        } else if (c == 'v') {
           (*position)[1]--;
        } else {
            // Safety Case
            std::cout << "Found a different character: " << c;
        }

        set.insert(*position);

        // Swapping directions for each Santa
        isSanta = !isSanta;
    }

    std:: cout << "Number of houses that receive a unique present: " << set.size();
    file.close();  // Close the file

    return 0;


}