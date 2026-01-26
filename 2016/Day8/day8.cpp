#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <bitset>
#include <vector> 

const size_t COLS{50}; // Holds number of columns
const size_t ROWS{6}; // Holds number of rows

// Turns on all of the pixels in a rectangle which is A wide and B tall, at the top-left corner of the bit grid
void createRect(std::vector<std::bitset<COLS>>& bitGrid, size_t A, size_t B) {
    for(size_t y{0}; y < B; y++){
        for(size_t x{0}; x < A; x++) {
            bitGrid[y].set(x);
        }
    }
}

// Performs modulus in which negative values are converted to their positive counterparts
size_t mod(size_t pos, size_t shift, size_t size) {
    return size_t((int(pos) - int(shift) + int(size)) % int(size));
}

// Performing the rotate operation on a row or column in the bit grid by first copying the
// needed column or row bits and then using the mod function to find the index of the new value
// for that location and then using the value at that index in the copied set of bits. Copying is 
// used to preserve data that would otherwise be overwritten, but needed later
void rotate(std::vector<std::bitset<COLS>>& bitGrid, bool isRow, size_t pos, size_t shift) {
    if(isRow) { // Row Case

        // Copying Data
        std::bitset<COLS> copy{};
        for(size_t i{0}; i < COLS; i++) {
            copy.set(i, bitGrid[pos][i]);
        }

        // Updating the Row using the calculated indexes that reference values in the original copied data
        for(size_t i{0}; i < COLS; i++) {
            bitGrid[pos][i] = copy[mod(i, shift, COLS)];
        }
    } else { // Column case

        // Copying Data
        std::bitset<ROWS> copy{}; 
        for(size_t i{0}; i < ROWS; i++) {
            copy.set(i, bitGrid[i][pos]);
        }

        // Updating the Column using the calculated indexes that reference values in the original copied data
        for(size_t i{0}; i < ROWS; i++) {
            bitGrid[i][pos] = copy[mod(i, shift, ROWS)];
        }
    }
}

// Returns the number of Pixels that are on (true) in  whole bit grid
int numberOfOnPixels(std::vector<std::bitset<COLS>>& bitGrid) {
    int onPixelCount{0};
    for(const auto& bitset : bitGrid) {
        onPixelCount += bitset.count();
    }
    return onPixelCount;
}

int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string line{}; // Holds the line read from the file
    std::smatch match{}; // Holds matching groups in pattern
    std::regex pattern{"(rect) (\\d+)x(\\d+)|(rotate) (row|column) (x|y)=(\\d+) by (\\d+)"}; // Pattern for extracting data

    // First entry is the row number, second entry is the column number to a grid of Bits
    std::vector<std::bitset<COLS>> bitGrid(ROWS);

    // Reading file input line by line
    while(std::getline(file, line)) {
      if(std::regex_match(line, match, pattern)) {
        if(match[1].matched && match[1].str() == "rect") { // "Rect" operation
            createRect(bitGrid, size_t(std::stoi(match[2].str())), size_t(std::stoi(match[3].str())));
        } else if(match[4].matched && match[4].str() == "rotate") { // "Rotate Operation"
            bool isRow{match[5].str() == "row"};
            rotate(bitGrid, isRow, size_t(std::stoi(match[7].str())), size_t(std::stoi(match[8].str())));
        }
      }
    }

    std::cout << "Number of On Pixels: " << numberOfOnPixels(bitGrid) << std::endl; // Printing Output

    // Part 2: Seeing the letters that would be printed to the screen and make a code
    std::cout << "Part 2 Output: " << std::endl;
    for(const auto& bitset : bitGrid) {
        std::string strLine{bitset.to_string('.', '#')}; // Converting a line of the grid (bitset) to string representation in which
        // 0 is '.' and 1 is '#'
        std::reverse(strLine.begin(), strLine.end()); // Reversing the created string because visually bitset indices go from right to left 
        // (least signifigant to most signifigant), but the letters are written left to right
        std::cout << strLine << std::endl; 
    }
    
    return 0;
}
