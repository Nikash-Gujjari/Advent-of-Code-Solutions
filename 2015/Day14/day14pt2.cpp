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

// Keeps track of the status of each reindeeer (distance travelled, points, resting status, and time in current status)
struct ReindeerStatus {
    int distance;
    int points;
    bool isResting;
    int statusTime;
};

// Finds reindeer(s) that have gone the furthest distance and awards a point to them. 
void award_points(std::unordered_map<std::string, ReindeerStatus>& reindeer_status, int furthest_distance) {
    for(auto& [_, reindeer] : reindeer_status) {
        reindeer.points += (reindeer.distance == furthest_distance) ? 1 : 0;
    }
}

int most_points(std::unordered_map<std::string, ReindeerStatus>& reindeer_status) {
    int max_points{0}; // Keeps track of the maximum points
    for(auto& [_, reindeer] : reindeer_status) {
        max_points = std::max(max_points, reindeer.points);
    }
    return max_points;
}

int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::regex pattern{"(\\w+) can fly (\\d+) km/s for (\\d+) seconds, but then must rest for (\\d+) seconds."};
    std::smatch match; // Holds matching groups in pattern 
    std::string line{}; // Holds the line read from the file

    std::unordered_map<std::string, ReindeerInfo> flyingInformation{};


    // Keeps track of each reindeer's distance, status of being in motion or resting, time in either state, and points awarded 
    std::unordered_map<std::string, ReindeerStatus> reindeer_status{};

    // Reading in line by line
    while (std::getline(file, line)) {

        // Storing data in flyingInformation and reindeer_status for each line of reindeer
        // information
        if (std::regex_search(line, match, pattern)) {
            flyingInformation[match[1]] = ReindeerInfo{std::stoi(match[2]), std::stoi(match[3]), std::stoi(match[4])};
            reindeer_status[match[1]] = ReindeerStatus{0, 0, false, 0};
        }       
    }

    // Time given for the reindeer to travel
    int reindeerTravelTime{2503};

    int current_max_distance{0}; //Keeps track of distance the leading reindeer has travelled

    // Every second a reindeer's status, status time, points, and distance travelled are updated accordingly
    for(int i{0}; i <= reindeerTravelTime; i++) {
        for(const auto& [name, reindeer] : flyingInformation) {
            if((reindeer_status[name].isResting && reindeer_status[name].statusTime == reindeer.restDuration) || 
                (!reindeer_status[name].isResting && reindeer_status[name].statusTime == reindeer.flyDuration)) {
                reindeer_status[name].isResting = !reindeer_status[name].isResting;
                reindeer_status[name].statusTime = 0;
            }
            
            // If the reindeer is not resting, then it moves its distance
            if(!reindeer_status[name].isResting) {
                reindeer_status[name].distance += reindeer.speed;
            }

            // Maintaing time of any status
            reindeer_status[name].statusTime += 1;

            // Finding the max distance a reindeer has travelled
            current_max_distance = std::max(current_max_distance, reindeer_status[name].distance);
        }

        // Give points the furthest reindeer(s)
        award_points(reindeer_status, current_max_distance);

    }

    // Find the highest number of points some reindeer(s) have obtained
    std::cout << "Reindeer with maximum points has: " << most_points(reindeer_status) << " points"; 

    return 0;

}