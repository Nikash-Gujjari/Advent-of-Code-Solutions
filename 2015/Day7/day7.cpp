#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <unordered_map>
#include <cstdint>


// Holds the Operation Type of an Instruction
enum OpType {
    ASSIGN,
    NOT,
    AND,
    OR,
    LSHIFT,
    RSHIFT
};

// Represents an Instruction as an Operation with 1 or 2 arguments. 
struct Instruction {
    OpType op;      
    std::string arg1;    // wire name or number as a string
    std::string arg2;   // optional
};


std::map<std::string, Instruction> instructions; // Global reference of inputed instructions
std::map<std::string, std::uint16_t> cache; // As Instruction expressions are evaluated into numbers, the cache holds their values

// Determines if a string is a number. This is useful for seeing if a token argument is a number or is an expression (non-number)
bool isNumber(const std::string& s){
    return !s.empty() &&
       std::all_of(s.begin(), s.end(),
                   [](unsigned char c){ return std::isdigit(c); });
}

std::uint16_t evaluate(const std::string& wire);  // forward-declare evaluate, since resolve and evaluate both call each other

// Checks if a token is an expression or number. If it is a number, no processing is needed and the number is obtained through 
// a conversion from string to unsigned 16 bit int. If it not a number, the evaluate function is called to recursively evalute the (expression)
// token
std::uint16_t resolve(const std::string& token) {
    if(isNumber(token)) {
        return static_cast<std::uint16_t>(std::stoi(token));
    } else {
        return evaluate(token);
    }
}

// A recursive function that evaluates the instruction tied to a wire name. 
std::uint16_t evaluate(const std::string& wire) {
    // If our cache has the wire's name, then we already know the value on the wire and can return the value found in the cache
    if(cache.count(wire) != 0) {
        return cache[wire];
    }

    Instruction wire_instruct{instructions.at(wire)}; // Holds the Instruction associated with the wire name
    std::uint16_t value{}; // Holds the value from the expression

    // Computing each instruction based on its operation and arg(s)
    switch(wire_instruct.op) {
        case ASSIGN:
            value = resolve(wire_instruct.arg1);
            break;
        case NOT:
            value = ~resolve(wire_instruct.arg1);
            break;
        case AND:
            value = resolve(wire_instruct.arg1) & resolve(wire_instruct.arg2);
            break;
        case OR:
            value = resolve(wire_instruct.arg1) | resolve(wire_instruct.arg2);
            break;
        case LSHIFT:
            value = resolve(wire_instruct.arg1) << std::stoi(wire_instruct.arg2);
            break;
        case RSHIFT:
            value = resolve(wire_instruct.arg1) >> std::stoi(wire_instruct.arg2);
            break;
    }
    
    value = value & 0xFFFF;   // keep 16 bits for overflowing values
    cache[wire] = value; // Storing computed result in cache for future uses
    return value;
}








int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string line{}; // Holds the line read from the file

    // Using multiple regex patterns for each operation written in the input
    std::regex assign_re("^(\\w+) -> (\\w+)$");
    std::regex not_re("^NOT (\\w+) -> (\\w+)$");
    std::regex andor_re("^(\\w+) (AND|OR) (\\w+) -> (\\w+)$");
    std::regex shift_re("^(\\w+) (LSHIFT|RSHIFT) (\\d+) -> (\\w+)$");

    std::smatch match; // Holds matching groups in pattern


    // Reading in line by line
    while (std::getline(file, line)) {

        // Checking for a regex match and creating an appropriate key-value pair 
        if (std::regex_match(line, match, assign_re)) {
            instructions[match[2].str()] = Instruction{ASSIGN, match[1].str(), ""};
        }
        else if (std::regex_match(line, match, not_re)) {
            instructions[match[2].str()] = Instruction{NOT, match[1].str(), ""};
        }

        else if (std::regex_match(line, match, andor_re)) {
            instructions[match[4].str()] = (match[2].str() == "AND") ? Instruction{AND, match[1].str(), match[3].str()} :   
                Instruction{OR, match[1].str(), match[3].str()};   
        }

        else if (std::regex_match(line, match, shift_re)) {
            instructions[match[4].str()] = (match[2].str() == "LSHIFT") ? Instruction{LSHIFT, match[1].str(), match[3].str()} :   
                Instruction{RSHIFT, match[1].str(), match[3].str()}; 
        }
    }
    file.close();

    uint16_t answer{evaluate("a")}; // Starts the recursive evaluation

    std::cout << "The value of a = " << answer << std::endl;

    // Part 2

    // Setting b to the value of a from part 1, resetting the cache, and recalculating a
    instructions["b"] = Instruction{ASSIGN, std::to_string(answer), ""};
    cache.clear();
    uint16_t part2_answer = evaluate("a");
    
    std::cout << "Part 2: The value of a = " << part2_answer << "\n";

    return 0;

}