#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>


const int COLS{100};
const int ROWS{100};

void updateBit(const std::bitset<COLS*ROWS>& current_matrix, std::bitset<COLS*ROWS>& new_matrix, int position){

    int col{position % COLS};
    int row{position / COLS};

    int adjacentOn{0};

    // Checking if all 8 neighbors are on, assuming their position is in bound
    if(col - 1 >= 0) {
        adjacentOn += (current_matrix.test(row * COLS + col - 1)) ? 1 : 0;
    }

    if(col + 1 < COLS) {
        adjacentOn += (current_matrix.test(row * COLS + col + 1)) ? 1 : 0;
    }

    if(row - 1 >= 0) {
        adjacentOn += (current_matrix.test((row - 1) * COLS + col)) ? 1 : 0;
        if(col - 1 >= 0) {
            adjacentOn += (current_matrix.test((row - 1) * COLS + col - 1)) ? 1 : 0;
        }
        if(col + 1 < COLS) {
            adjacentOn += (current_matrix.test((row - 1) * COLS + col + 1)) ? 1 : 0;
        } 
    }

    if(row + 1 < ROWS) {
        adjacentOn += (current_matrix.test((row + 1) * COLS + col)) ? 1 : 0;
        if(col + 1 < COLS) {
            adjacentOn += (current_matrix.test((row + 1) * COLS + col + 1)) ? 1 : 0;
        }
        if(col - 1 >= 0) {
            adjacentOn += (current_matrix.test((row + 1) * COLS + col - 1)) ? 1 : 0;
        }  
    }

    // Updating new_matrix accordingly based on the rules
    if(current_matrix.test(position)) {
        if(adjacentOn == 2 || adjacentOn == 3) {
            new_matrix.set(position);
        }
    } else if(adjacentOn == 3) { 
        new_matrix.set(position);
    }
}


int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }



    std::bitset<ROWS*COLS> current_matrix{}; // Will represent a 
    // matrix of the lights through a flatened, single bitset. 
    // Data is converted:
    // {x1, y1} -> current_matrix[x1*COLS + y1]
    
     
    // Reading in line by line. By default bitsets are all 0's so we only
    // need to flip bits in places where there the light is on, represented
    // by '#'. 

    std::string line{}; // Holds the line read from the file

    int curr_row{0}; // Keeps track of the current row
    while (std::getline(file, line)) { 
        for(size_t i{0}; i < line.size(); i++) {
            if(line[i] == '#') {
                current_matrix.set(curr_row * COLS + i);
            }
        }
        curr_row++;
    }

    // Part 2: Setting the 4 corner lights on current_matrix
    current_matrix.set(0);
    current_matrix.set(COLS - 1);
    current_matrix.set((ROWS - 1) * COLS);
    current_matrix.set(ROWS * COLS - 1);


    // Doing 100 step transformation
    for(int i{0}; i < 100; i++) {
        std::bitset<ROWS * COLS> new_matrix{}; // Holds the calculated transformation
        // of current_matrix after a step

        // Setting 4 cornor lights on new_matrix
        new_matrix.set(0);
        new_matrix.set(COLS - 1);
        new_matrix.set((ROWS - 1) * COLS);
        new_matrix.set(ROWS * COLS - 1);

        // Looping through all bits and updating. Ignoring corner lights so that
        // they always stay on. 
        for(int j{1}; j < (ROWS * COLS) - 1; j++) {
            if(j != COLS - 1 && j != (ROWS - 1) * COLS) {
                updateBit(current_matrix, new_matrix, j);
            }
        }

        current_matrix = new_matrix;

    }

    file.close();

    // Number of lights that are on at the end
    std::cout << "Number of lights on after 100 steps: " << current_matrix.count();


    return 0;



}