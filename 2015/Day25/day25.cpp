#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <climits>
#include <regex>


// Increments the Row and Column number on the diagonal correctly
void incrementLocation(size_t& currRow, size_t& currColumn) {
    if(currRow == 0) {
        size_t temp{currRow};
        currRow = currColumn + 1;
        currColumn = temp;
        
    } else {
        currColumn += 1;
        currRow -= 1;
    }
}


int main() {

    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }
    
    std::smatch match{}; // Holds matching groups in pattern 
    std::string line{}; // Holds the line read from the file

    size_t soughtRowNumber{0};
    size_t soughtColumnNumber{0};

    // Reading line by line
    while (std::getline(file, line)) { 
         // Finding row and column number
        std::regex pattern{"To continue, please consult the code grid in the manual.  Enter the code at row (\\d+), column (\\d+)."}; // Pattern for extracting data
        if (std::regex_search(line, match, pattern)) {
            // Since our grid is 0 indexed, we need to subtract 1. 
            soughtRowNumber = std::stoi(match[1].str()) - 1;
            soughtColumnNumber = std::stoi(match[2].str()) - 1;
        }
    }
    file.close();

    // Creating the grid. 
    std::vector<std::vector<unsigned long long>> grid{
        std::vector<unsigned long long>{20151125, 18749137, 17289845, 30943339, 10071777, 33511524},
        std::vector<unsigned long long>{31916031, 21629792, 16929656, 7726640, 15514188, 4041754},
        std::vector<unsigned long long>{16080970, 8057251, 1601130, 7981243, 11661866, 16474243},
        std::vector<unsigned long long>{24592653, 32451966, 21345942, 9380097, 10600672, 31527494},
        std::vector<unsigned long long>{77061, 17552253, 28094349, 6899651, 9250759, 31663883},
        std::vector<unsigned long long>{33071741, 6796745, 25397450, 24659492, 1534922, 27995004}
    }; 

    // Starting at the last full diagonal
    size_t prevRow{0};
    size_t prevColumn{5};


    // Iterating until we have calculated the sought number at the given Row and Column
    while(prevRow != soughtRowNumber || prevColumn != soughtColumnNumber) {
        size_t currRow{prevRow};
        size_t currColumn{prevColumn};
        incrementLocation(currRow, currColumn);
        // Creating a new row, if needed
        if (currRow >= grid.size()) {
            grid.push_back({});
        }
        // Creating a new column, if needed
        if (currColumn >= grid[currRow].size()) {
            grid[currRow].resize(currColumn + 1);
        }
        grid[currRow][currColumn] = (grid[prevRow][prevColumn] * 252533) % 33554393; // Calculating next number
        prevRow = currRow;  // Updating the previous row and column numbers for next iteration
        prevColumn = currColumn;
    }

    std::cout << "Code to give to machine: " << grid[prevRow][prevColumn];
   
    return 0;

}