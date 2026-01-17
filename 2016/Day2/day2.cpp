#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    int curr_num{5}; // Tracks current number location on keypad
    std::string bathroomCode{""}; // Stores each digit of the bathroom code
    std::string line{};

    // Reading file input line by line
    while(std::getline(file, line)) {
        for(const char& dir: line) {

            // Using these numeric comparisons to find the keypad boudnaries
            // to see which operations would actually change the location
            switch(dir) {
                case 'U':
                    if(curr_num >= 4) {
                        curr_num -= 3;
                    }
                    break;
                case 'L':
                    if(curr_num % 3 != 1) {
                        curr_num -= 1;
                    }
                    break;
                case 'R':
                    if(curr_num % 3 != 0) {
                        curr_num += 1;
                    }
                    break;
                case 'D':
                    if(curr_num <= 6) {
                        curr_num += 3;
                    }
                    break;
                
            }
        }

        bathroomCode.append(std::to_string(curr_num)); // At the end of a line append which number location it is at
    }

    std::cout << "Bathroom Code: " << bathroomCode;
   
    return 0;


}
