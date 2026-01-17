#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>

int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    char curr_num{'5'}; // Tracks current number location on keypad
    std::string bathroomCode{""}; // Stores each digit of the bathroom code
    std::string line{}; // Holds the line read from the file

    // A map that represents transitions from a given keypad number and direction
    std::unordered_map<char, std::unordered_map<char, char>> transitions{
        {'1', std::unordered_map<char, char>{
            {'U', '1'}, {'L', '1'}, {'R', '1'}, {'D', '3'}}},
        {'2', std::unordered_map<char, char>{
            {'U', '2'}, {'L', '2'}, {'R', '3'}, {'D', '6'}}},
        {'3', std::unordered_map<char, char>{
            {'U', '1'}, {'L', '2'}, {'R', '4'}, {'D', '7'}}},
        {'4', std::unordered_map<char, char>{
            {'U', '4'}, {'L', '3'}, {'R', '4'}, {'D', '8'}}},
        {'5', std::unordered_map<char, char>{
            {'U', '5'}, {'L', '5'}, {'R', '6'}, {'D', '5'}}},
        {'6', std::unordered_map<char, char>{
            {'U', '2'}, {'L', '5'}, {'R', '7'}, {'D', 'A'}}},
        {'7', std::unordered_map<char, char>{
            {'U', '3'}, {'L', '6'}, {'R', '8'}, {'D', 'B'}}},
        {'8', std::unordered_map<char, char>{
            {'U', '4'}, {'L', '7'}, {'R', '9'}, {'D', 'C'}}},
        {'9', std::unordered_map<char, char>{
            {'U', '9'}, {'L', '8'}, {'R', '9'}, {'D', '9'}}},
        {'A', std::unordered_map<char, char>{
            {'U', '6'}, {'L', 'A'}, {'R', 'B'}, {'D', 'A'}}},
        {'B', std::unordered_map<char, char>{
            {'U', '7'}, {'L', 'A'}, {'R', 'C'}, {'D', 'D'}}},
        {'C', std::unordered_map<char, char>{
            {'U', '8'}, {'L', 'B'}, {'R', 'C'}, {'D', 'C'}}},
        {'D', std::unordered_map<char, char>{
            {'U', 'B'}, {'L', 'D'}, {'R', 'D'}, {'D', 'D'}}}
        };


    // Reading file input line by line
    while(std::getline(file, line)) {
        // Keep transitioning the keypad number location
        for(const char& dir: line) {
            curr_num = transitions[curr_num][dir]; 
        }

        bathroomCode.push_back(curr_num); // At the end of an instruction line append which number location we end at
    }

    std::cout << "Bathroom Code: " << bathroomCode;
   
    return 0;


}
