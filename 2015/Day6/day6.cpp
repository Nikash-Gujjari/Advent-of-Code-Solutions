#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <cstdint>


// Custom data structure to hold a memory efficent bitmatrix, in which 1 represents turned on light
// and 0 represents turned off light. It uses bitwise operations and stores data within 64 bit blocks
// that are within a row of uint_64 vecto; stride((cols + 63) / 64) sets the number of blocks 
// per row and is equivalent, but faster than,
// ceil(cols / 64.0)
class BitMatrix {
public:
    BitMatrix(size_t rows, size_t cols)
        : rows(rows), cols(cols),
          stride((cols + 63) / 64),
          data(rows * stride, 0ULL) {} // 0's out all data
    // It stores data as follows to help densly and compactly store data:
    //Row0: [64 bits][64 bits][64 bits]...
    //Row1: [64 bits][64 bits][64 bits]...
    //...
    // 

    // r * stride + (c >> 6) : This picks the exact 64-bit block that contains the bit we want.
    // (1ULL << (c & 63)): This computes a mask with only the target bit set and is faster than
    // c / 64. 

    // Sets the bit to 1
    inline void turnOn(size_t x1, size_t x2, size_t y1, size_t y2) {
        for(size_t i{x1}; i <= x2; i++) {
            for(size_t j{y1}; j <= y2; j++) {
                data[i * stride + (j >> 6)] |= (1ULL << (j & 63));
            }
        }
 
    }

    // Sets the bit to 0
    inline void turnOff(size_t x1, size_t x2, size_t y1, size_t y2) {
        for(size_t i{x1}; i <= x2; i++) {
            for(size_t j{y1}; j <= y2; j++) {
                data[i * stride + (j >> 6)] &= ~(1ULL << (j & 63));
            }
        }
    }

    // Flips the bit 
    inline void toggle(size_t x1, size_t x2, size_t y1, size_t y2) {
        for(size_t i{x1}; i <= x2; i++) {
            for(size_t j{y1}; j <= y2; j++) {
                data[i * stride + (j >> 6)] ^= (1ULL << (j & 63));
            }
        }
    }

    // Returns the bit
    inline bool get(size_t r, size_t c) const {
        return (data[r * stride + (c >> 6)] >> (c & 63)) & 1ULL;
    }

    // Counts all the 1's or "On" states in a 64 bit block
    uint64_t popcount64(uint64_t x) const {
        std::uint64_t count{0};
        while (x) {
            x &= (x - 1);  // clear the lowest set bit
            count++;
        }
        return count;
    }

     // --- Count total number of 1's --- across entire data vector
    uint64_t countOn() const {
        std::uint64_t total{0};
        for (uint64_t block : data)
            total += popcount64(block);
        return total;
    }

    


private:
    int rows, cols;
    int stride; // number of uint64_int blocks per row
    std::vector<std::uint64_t> data;
};

int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    BitMatrix lightMatrix{1000, 1000}; // Represents light in the grid, with lights being on or off represented as a bit value
    std::string line{}; // Holds the line read from the file
    std::regex pattern{"(turn on|turn off|toggle) (\\d+),(\\d+) through (\\d+),(\\d+)"};
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
            x1 = static_cast<size_t>(std::stoull(match[2].str()));
            y1 = static_cast<size_t>(std::stoull(match[3].str()));
            x2 = static_cast<size_t>(std::stoull(match[4].str()));
            y2 = static_cast<size_t>(std::stoull(match[5].str()));
            
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

    std::cout << "Number of Turned On Lights: " << lightMatrix.countOn();

    return 0;

}