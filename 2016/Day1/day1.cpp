#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_set>


// Holds Directions. Directions change by doing addition for turning right (R) or subtraction for turning left (L)
// then taking mod 4 of the result.
enum Direction {
    North = 0,
    East = 1,
    South = 2,
    West = 3
};

// My function to do modulo with only positive results. Negative numbers that would be returned by C++'s "%"
// operator are converted to their positive counterparts
int positive_mod(int a, int b) {
    int r = a % b;
    return (r < 0) ? r + b : r;
}

// Custom hash function needed to make an unordered (hash) set of type pair<int, int>
struct pair_hash {
    std::size_t operator() (const std::pair<int,int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::vector<std::string> directions{};
    size_t start{0};
    size_t end{};

    std::string line{};
    std::getline(file, line);
    
    std::string delim{", "};
    while((end = line.find(delim, start)) != std::string::npos) {
        directions.push_back(line.substr(start, end - start));
        start = end + delim.length();
    }
    directions.push_back(line.substr(start));

    int x_cord{0};
    int y_cord{0};
    Direction yourDirection{North};

    std::unordered_set<std::pair<int, int>, pair_hash> visited{std::pair<int, int>{0, 0}};

    std::smatch match{};
    std::regex pattern{"(L|R)(\\d+)"};
    for(std::string direction : directions) {
        if(std::regex_match(direction, match, pattern)) {
            // Using positive modulus function to maintain direction based on the Direction Enum's int values
            if(match[1].str() == "L") {
                yourDirection = Direction(positive_mod(yourDirection - 1, 4));
            } else {
                yourDirection = Direction(positive_mod(yourDirection + 1, 4));
            }

            int steps{std::stoi(match[2].str())};

            // Part 1
            // Incrementing appropriate coordinate based on direction
            // switch(yourDirection) {
            //     case North:
            //         y_cord += steps;
            //         break;
            //     case East:
            //         x_cord += steps;
            //         break;
            //     case South:
            //         y_cord -= steps;
            //         break;
            //     case West:
            //         x_cord -= steps;
            //         break;
            // }

            // Part 2
            // Incrementing location step by step, based on direction, and adding that new location to 
            // the visited data structure
            for(int i{0}; i < steps; i++) {
                switch(yourDirection) {
                case North:
                    y_cord++;
                    break;
                case East:
                    x_cord++;
                    break;
                case South:
                    y_cord--;
                    break;
                case West:
                    x_cord--;
                    break;
                }
                auto pos = std::pair<int, int>{x_cord, y_cord};

                // If a location we are at is already is in visted, then we have found the actual
                // Easter Bunny HQ
                if(visited.count(pos) != 0) {
                    std::cout << "Easter Bunny HQ is " << std::abs(x_cord) + std::abs(y_cord) << " steps away";
                    return 0;
                } 


                visited.insert(pos); // Updating visited
            }
            
        }

    }

    // Part 1 Output
    std::cout << "Easter Bunny HQ is " << std::abs(x_cord) + std::abs(y_cord) << " steps away";
    return 0;


}
