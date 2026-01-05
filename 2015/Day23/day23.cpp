#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>

// Represents an Instruction
struct Instruction {
    std::string type;
    std::vector<std::string> args;
};

// Updates instruction number, a, and b based on the instruction
void processInstruction(const Instruction& instruction, int& instruction_number, std::unordered_map<std::string, int>& registers) {
    if(instruction.type == "hlf") {
        registers[instruction.args[0]] /= 2;
        instruction_number++;
    } else if(instruction.type == "tpl") {
        registers[instruction.args[0]] *= 3;
        instruction_number++;
    } else if(instruction.type == "inc") {
        registers[instruction.args[0]]++;
        instruction_number++;
    } else if(instruction.type == "jmp") {
        instruction_number = instruction_number + std::stoi(instruction.args[0]);
    } else if(instruction.type == "jie") {
        instruction_number = (registers[instruction.args[0]] % 2 == 0) ? instruction_number + std::stoi(instruction.args[1]) : instruction_number + 1;
    } else if(instruction.type == "jio") {
        instruction_number = (registers[instruction.args[0]] == 1) ? instruction_number + std::stoi(instruction.args[1]) : instruction_number + 1;
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


    std::vector<Instruction> instructions{}; // Holds the list of instructions from input


    // Reading line by line
    while (std::getline(file, line)) { 


        // Processing all information into instructions vector
        std::regex pattern{"(\\S+) ([^\\s,]+),?\\s?(\\S+)?"}; // Pattern for extracting data

        // Creating Instructions based on matching pattern
        if (std::regex_search(line, match, pattern)) {
            Instruction curr{};
            curr.type = match[1].str();
            std::vector<std::string> args{};

            // Adding arguments if they are present

            // There is always at least 1 argument
            args.push_back(match[2].str());

            // Optional second argument 
            if(match[3].matched) {
                args.push_back(match[3].str());
            }           

            curr.args = args;

            instructions.push_back(curr);            
        }   

    }
    file.close();

    // Creating a map to represent registers and populating it with default values for a and b
    std::unordered_map<std::string, int> registers{
        {"a", 1}, //  For Part 2, this is set to 1. Set this to 0 for Part 1.
        {"b", 0}
    };

    int curr_instruction_number{0}; // Holds the current instruction being processed

    // Keep processing instructions until our instruction number goes to an index outside of the instructions vector
    while(curr_instruction_number < int(instructions.size())) {
        processInstruction(instructions[curr_instruction_number], curr_instruction_number, registers);
    }

    std::cout << "Value  in register b when program finishes: " << registers["b"];


    return 0;

}