#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>


// Custom data structure that represents the grid of lights, in a flat vector, with each entry 
// representing brightness
class BrightnessMatrix {
public:
    BrightnessMatrix(size_t rows, size_t cols)
        : rows(rows), cols(cols), data(rows * cols, 0) {}
    // It stores data as follows to help densly and compactly store data:
    // {[0,0]},{[0,1]}...{[0,999]},{[1,0]}...

    // The conversion scheme is:
    // {x1, y1} -> data[x1*1000 + y1]

    // Increases brightness by 1
    inline void turnOn(size_t x1, size_t x2, size_t y1, size_t y2) {
        for(size_t i{x1}; i <= x2; i++) {
            for(size_t j{y1}; j <= y2; j++) {
                data[i * rows + j]++;
            }
        }
 
    }

    // Decreases brightness by 1, with a minimum at 0
    inline void turnOff(size_t x1, size_t x2, size_t y1, size_t y2) {
        for(size_t i{x1}; i <= x2; i++) {
            for(size_t j{y1}; j <= y2; j++) {
                data[i * rows + j] = std::max(data[i*1000 + j] - 1, 0);
            }
        }
    }

    // Increases brightness by 2
    inline void toggle(size_t x1, size_t x2, size_t y1, size_t y2) {
        for(size_t i{x1}; i <= x2; i++) {
            for(size_t j{y1}; j <= y2; j++) {
                data[i * rows + j]+= 2;
            }
        }
    }

    // Returns the bit
    inline int get(size_t r, size_t c) const {
        return data[r*1000 + c];
    }

    inline int countBrightness() const {
        int total{0};
        for (const int& x: data)
            total += x;
        return total;
    }

    


private:
    int rows, cols;
    std::vector<int> data;
};

int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    BrightnessMatrix lightMatrix(1000, 1000); // Represents light in the grid, with lights being on or off represented as a bit value
    std::string line{}; // Holds the line read from the file
    std::regex pattern("(turn on|turn off|toggle) (\\d+),(\\d+) through (\\d+),(\\d+)");
    std::smatch match; // Holds matching groups in pattern
    std::string instructionType{""}; // Holds type of instruction: "turn on", "turn off", "toggle"

    // Hold Coordinates
    int x1{};
    int y1{};
    int x2{};
    int y2{};

    // Reading in line by line
    while (std::getline(file, line)) {
        if (std::regex_search(line, match, pattern)) {
            instructionType = match[1].str();
            x1 = std::stoi(match[2].str());
            y1 = std::stoi(match[3].str());
            x2 = std::stoi(match[4].str());
            y2 = std::stoi(match[5].str());
            
            if(instructionType == "turn on") {
                lightMatrix.turnOn(x1, x2, y1, y2);
            } else if(instructionType == "turn off") {
                lightMatrix.turnOff(x1, x2, y1, y2);
            } else if(instructionType == "toggle") {
                lightMatrix.toggle(x1, x2, y1, y2);
            } else {
                std:: cout << "Invalid Instruction Found\n"; 
            }
        }
    }
    file.close();

    std::cout << "Total Brightness: " << lightMatrix.countBrightness();

}