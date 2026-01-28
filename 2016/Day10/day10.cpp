#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <bitset>
#include <regex>
#include <cstdint>
#include <algorithm>

// Input Data
const size_t NUM_BOTS{210};
const std::uint8_t SOUGHT_LOW{17};
const std::uint8_t SOUGHT_HIGH{61};

// Forward Declaration 
struct Instruction;
struct Bot;

// Global Variables needed for executing instructions between bots and/or outputs
std::vector<Bot> bots(NUM_BOTS); // Holds all the bots. Bots numbers correlate to the vector's index
std::vector<std::uint8_t> outputBin(NUM_BOTS); // Holds all the outputs. Output numbers correlate to the vector's index

// Represents an Instruction
struct Instruction {
    size_t lowChipDestinationNumber{}; // The Bot/Output number that will receive the low chip
    size_t highChipDestinationNumber{}; // The Bot/Output number that will receive the high chip
    std::bitset<2> destinations{}; // 0 index correlates to lowChip and 1st index correlates to highChip. A '0' bit means that the destination is
    // another bot, while a '1' bit means the destination is for an output bin
}; 

// Represents a Bot's state
struct Bot {
    std::optional<std::uint8_t> lowChip{}; // The value of the bot's low chip, if set
    std::optional<std::uint8_t> highChip{}; // The value of the bot's high chip, if set
    std::optional<Instruction> instruction{}; // A bot's instruction, if set
    
    // Checks if a bot is holding 2 chips; true if so, false otherwise
    bool has2Chips() {
        return lowChip.has_value() && highChip.has_value();
    }

    // Adds a new chip to the bot by comparing its previous chip value, if present, and determing which chip values are now the high and low value chips.
    // If the bot did not hold any chips, then the new chip has its value set for lowChip. 
    // The function returns true if when adding a new chip it ended up comparing the two sought after chip values; otherwise, it returns false
    bool addChip(std::uint8_t newChipValue) {
        if(has2Chips()) { // A bot can not hold more than 2 chips, so this is an error case
            std::cerr << "Error trying to add a chip to Bot that already has 2 chips";
            std::exit(1);
        } else if(lowChip.has_value()) { // Checking if a bot already holds a chip, which would be in the lowChip variable because of the default assignment
            // when there are no chipas

            // Doing comparisons to determine new high and low value chips
            if(newChipValue < lowChip.value()) {
                highChip = lowChip.value();
                lowChip = newChipValue;
            } else {
                highChip = newChipValue;
            }

            // Checking if the comparisons lead to comparing the 2 sought after chip values (input to Part 1). If so, return true           
            if(highChip.value() == SOUGHT_HIGH && lowChip.value() == SOUGHT_LOW) {
                return true;
            }
            
        } else {
            lowChip = newChipValue; // Default case for when the bot has no chips. The new chip's value is set for lowChip
        } 

        return false; // Return false if the sought after comparison is not done 

    }

    // Executes a bots instruction by updating the state of its chips and the chips of its instruction's destination
    // If the movement of a chip causes a receiving (destination) bot to do the sought after comparison (communicated through addChip's return value),
    // then that receiving bot's number is returned; otherwise, -1 is returned. 
    std::int16_t executeInstruction() {
        // Getting variables from the instruction object
        auto destinations = instruction.value().destinations;
        size_t lowDestination = instruction.value().lowChipDestinationNumber;
        size_t highDestination = instruction.value().highChipDestinationNumber;

        // Holds the boolean results of addChip to figure out for which bot the sought after comparison happened
        bool soughtComparisonHappenedLow{false}; 
        bool soughtComparisonHappenedHigh{false};

        // Processing lowChip move
        if(destinations[0]) { // output bin case
            outputBin[lowDestination] = lowChip.value();
            
        } else { // other bot case
            soughtComparisonHappenedLow = bots[lowDestination].addChip(lowChip.value());
        }
        lowChip = std::nullopt; // Removing low chip from this bot's state

        // Processing highChip move
        if(destinations[1]) { // output bin case
            outputBin[highDestination] = highChip.value();
            highChip = std::nullopt;
        } else { // other bot case
            soughtComparisonHappenedHigh = bots[highDestination].addChip(highChip.value());
        }
        highChip = std::nullopt; // Removing high chip from this bot's state
        
        // Checking if either the highChip or LowChip move made the sought after comparison happen. If either is true
        // then the bot number, the destination of the relevant chip from this executeInstruction method, is returned. Otherwise
        // -1 is returned. 
        if(soughtComparisonHappenedLow) {
            return lowDestination;
        } else if(soughtComparisonHappenedHigh) {
            return highDestination;
        } else {
            return -1;
        }

       
    }
};

// Pretty print function for a vector
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i + 1 < v.size()) os << ", ";
    }
    os << "]";
    return os;
}


int main() {

    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    
    std::string line{}; // Holds the line read from the file
    std::smatch match{}; // Holds matching groups in pattern


    // Patterns for extracting Data
    std::regex pattern1{"value (\\d+) goes to bot (\\d+)"};
    std::regex pattern2{"bot (\\d+) gives low to (bot|output) (\\d+) and high to (bot|output) (\\d+)"};

    // Reading file input line by line
    while(std::getline(file, line)) {
      if(std::regex_match(line, match, pattern1)) { // Pattern 1, which is when a bot gets a disk from the input bin

        // Converting from string to appropriate data type
        size_t botNumber{static_cast<size_t>(std::stoul(match[2].str()))};
        std::uint8_t chipValue{static_cast<uint8_t>(std::stoul(match[1].str()))};

        bots[botNumber].addChip(chipValue); // Adding the chip to teh bot
      } else if(std::regex_match(line, match, pattern2)) { // Pattern 2, which is when a bot is given an instruction on where to move
        // its disks

        // Converting from string to appropriate data type
        size_t botNumber{static_cast<size_t>(std::stoul(match[1].str()))};
        size_t lowtargetNumber{static_cast<size_t>(std::stoul(match[3].str()))};
        size_t hightargetNumber{static_cast<size_t>(std::stoul(match[5].str()))};

        // Creating an instruction struct to hold the data
        Instruction instr{};
        instr.lowChipDestinationNumber = lowtargetNumber;
        instr.highChipDestinationNumber = hightargetNumber;
        if(match[2].str() == "output") {
            instr.destinations.set(0);
        }
        if(match[4].str() == "output") {
            instr.destinations.set(1);
        }

        bots[botNumber].instruction = instr; // Assigning the created instruction
      }
    }

    bool foundABotThatWillExecute{false}; // Variable that checks whether there was at least 1 bot that actually executed its instruction after checking each bot
    // to see if the while loop should be termianted
    while(true) {
        foundABotThatWillExecute = false; // Resetting after every iteration 
        for(Bot& bot : bots) {
            if(bot.has2Chips()) {
                foundABotThatWillExecute = true; // Since there is a bot with 2 chips that will execute its instruction, we set thsi value to true
                std::int16_t ret_val{bot.executeInstruction()}; 
                if(ret_val != -1) { // Checking if the return value is not -1, which means we found a bot that compared the two given disk values
                    std::cout << "The machine that is responsible for comparing " << std::to_string(static_cast<unsigned int>(SOUGHT_LOW)) << " and " << \
                    std::to_string(static_cast<unsigned int>(SOUGHT_HIGH)) << ": " << ret_val << std::endl; // Printing part 1 output
                    
                }
            }
        }
        if(!foundABotThatWillExecute) { // If no bots are found to have executed in this iteration, then there is no more processing that can be done
            break;
        }
    }

    // Part 2
    std::vector<size_t> inputPt2{0, 1, 2};
    int product{1}; // Will hold the product of disk values in the output bins

    // Calculating product
    for(const size_t& inputIndex : inputPt2) {
        product *= outputBin[inputIndex];
    }
    std::cout << "[Part 2]: Multiplying the values for the given output bin indices " << inputPt2 << " : " <<product; // Printing Part 2 Output

    return 0;
}

