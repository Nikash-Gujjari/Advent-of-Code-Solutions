#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>

// Keeps track of the speed, flight duration, and rest time of each reindeer
struct ReindeerInfo {
    int speed;
    int flyDuration;
    int restDuration;

};

int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::regex pattern{
    "(\\w+) can fly (\\d+) km/s for (\\d+) seconds, but then must rest for (\\d+) seconds."};
    std::smatch match; // Holds matching groups in pattern 
    std::string line{}; // Holds the line read from the file

    std::unordered_map<std::string, ReindeerInfo> flyingInformation{}; 

    // Reading in line by line
    while (std::getline(file, line)) {

        // Storing data in flyingInformation for each reindeer's information being
        // read-in
        if (std::regex_search(line, match, pattern)) {
            flyingInformation[match[1].str()] = ReindeerInfo{std::stoi(match[2].str()), std::stoi(match[3].str()), std::stoi(match[4].str())};
        }       
    }

    file.close();

    // Time given for the reindeer to travel
    int reindeerTravelTime{2503};


    // Algorithm defines a full-cycle as the total time a reindeer can fly + its needed rest. The modulus from diving the given time
    // from the reindeer full-cycle time is analyzed to see how much movement the reindeer does. This movement is added to the
    // calculated distance from the reindeer's previous full-cycles to derive how much distance the reindeer has covered in total


    int last_cycles_seconds{0}; // Keeps Track of the last cycle
    int reindeer_distance_travelled{0}; // Keeps track of the distance a given reindeer has travelled
    int winning_reindeer_distance{0}; // Will hold the longest distance travelled by some reindeer

    for(const auto& [_, reindeer_info] : flyingInformation) {

        // Calculating the seconds on the last cycle
        last_cycles_seconds = reindeerTravelTime % (reindeer_info.flyDuration + reindeer_info.restDuration);

        // Calculating the distance travelled from all previous, full cycles
        reindeer_distance_travelled = (reindeerTravelTime / (reindeer_info.flyDuration + reindeer_info.restDuration)) * (reindeer_info.speed * reindeer_info.flyDuration);
        
        // Adds the distance from the last cycle by seeing if the reindeer is still in motion or not. If it is in motion last_cycle_seconds < flight duration
        // and distance is calculated accordingly. Otherwise, it must be at rest, having already travelled the max time and is resting. 
        reindeer_distance_travelled += reindeer_info.speed * std::min(last_cycles_seconds, reindeer_info.flyDuration);

        // Updating a greater reindeer distance, if found
        winning_reindeer_distance = std::max(winning_reindeer_distance, reindeer_distance_travelled);

        // Resetting this variable for next iteration
        reindeer_distance_travelled = 0;
         
    }

    std::cout << "The winning reindeer's distance: " << winning_reindeer_distance; 

    return 0;



}