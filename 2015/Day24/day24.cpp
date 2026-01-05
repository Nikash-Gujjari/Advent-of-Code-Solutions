#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <climits>


long long bestQESoFar{INT_MAX}; // Keeps track of minimum QUE
size_t bestGroupSizeSoFar{SIZE_MAX}; // Keeps track of minimum GroupSize

int targetWeight{0}; // Gives a global reference to target weight
bool part2{false}; // Gives a global reference about whether the part2 (4 Groups) situation is used

// Determine's the best (minimum) group size and best (minimum) QE for that size
void determineBestGroupSizeAndQE(const std::vector<int>& group){

    // Only consider a new group that is smaller than our best so far
    if(group.size() > bestGroupSizeSoFar) {
        return;
    }

    // Calculating QE of the group
    long long QE{1};
    for(const int& weight: group) {
        QE *= weight;
    }

    // If the new group is smaller, we consider it better than a bigger group, regardless of
    // QE calculation
    if(group.size() < bestGroupSizeSoFar) {
        bestQESoFar = QE;
        bestGroupSizeSoFar = group.size();
    } else {
        // For equal sized groups, we look for the smaller QE
        bestQESoFar = std::min(QE, bestQESoFar);
    }
}

// Calculates total group's weight
int getGroupWeight(const std::vector<int>& group) {
    int total{0};
    for(const int& weight: group) {
        total += weight;
    }
    return total;
}

// Gives a boolean answer to whether the group has the correct weight
bool correctWeight(const std::vector<int>& group) {
    return getGroupWeight(group) == targetWeight;
}

// Finds a third group for part 2
bool isThereThirdGroup(const std::vector<int>& package_weights, size_t pos, std::vector<int> thirdGroup) {
    // Third group is found, meaning a 4th group is valid as well
    if(correctWeight(thirdGroup)) {
        return true;
    }

    // Exhausting options
    if(pos >= package_weights.size()) {
        return false; 
    }

    // Pruning condition for packages that would put our group's weight above the target weight
    if(package_weights[pos] + getGroupWeight(thirdGroup) <= targetWeight) {
        std::vector<int> thirdGroupCopy = thirdGroup;
        thirdGroupCopy.push_back(package_weights[pos]);
        // Recursively making binary choices to put the package weight in the group or not
        return isThereThirdGroup(package_weights, pos + 1, thirdGroupCopy) || isThereThirdGroup(package_weights, pos + 1, thirdGroup);
    }

    // Since the package sizes are increasing, this could be a place to just return false
    return isThereThirdGroup(package_weights, pos + 1, thirdGroup);

}

// Finds if there is a second group. For part 2, an additional method that looks for a third group is called. If
// this function returns true, then there are other valid groups for our first group, and so our found first group
// can be used for minimum QE and group size calculations 
bool isThereSecondGroup(const std::vector<int>& package_weights, size_t pos, std::vector<int> secondGroup) {
    if(correctWeight(secondGroup))  {
        // For Part 2, we take the calculated second group's weights and remove them from possible package weights
        // for a third group that will be found in the isThereThirdGroup function.
        if(part2) {
            std::vector<int> weights_for_third_group;
            for (int x : package_weights) {
                if (std::find(secondGroup.begin(), secondGroup.end(), x) == secondGroup.end()) {
                    weights_for_third_group.push_back(x);
                }
            }
            std::vector<int> thirdGroup{};
            return isThereThirdGroup(weights_for_third_group, 0, thirdGroup);
        } else {
            // For Part 1, a second group is found so a valid third group also exists. 
            return true;
        }
        
    }

    // Exhausting options
    if(pos >= package_weights.size()) {
        return false; 
    }

    // Pruning condition for packages that would put our group's weight above the target weight
    if(package_weights[pos] + getGroupWeight(secondGroup) <= targetWeight) {
        std::vector<int> secondGroupCopy = secondGroup;
        secondGroupCopy.push_back(package_weights[pos]);
        // Recursively making binary choices to put the package weight in the group or not
        return isThereSecondGroup(package_weights, pos + 1, secondGroupCopy) || isThereSecondGroup(package_weights, pos + 1, secondGroup);
    }
    return isThereSecondGroup(package_weights, pos + 1, secondGroup);
}

// Finds the package weights left (not in the first group), to use for a second (and third for part 2) group.
bool possibleSecondGroup(const std::vector<int>& firstGroup, const std::vector<int>& package_weights) {
    std::vector<int> weights_for_second_group;

    for (int x : package_weights) {
        if (std::find(firstGroup.begin(), firstGroup.end(), x) == firstGroup.end()) {
            weights_for_second_group.push_back(x);
        }
    }
    std::vector<int> secondGroup{};
    return isThereSecondGroup(weights_for_second_group, 0, secondGroup);
}


// Recursive method that finds a first group that matches the target weight before looking for a second (and third for part 2) group.
void findFirstGroup(const std::vector<int>& package_weights, size_t pos, std::vector<int> firstGroup) {

    if(correctWeight(firstGroup) && possibleSecondGroup(firstGroup, package_weights)) {
        determineBestGroupSizeAndQE(firstGroup);
        return;
    }

    // Ending condition and pruning condition, respectively
    if(pos >= package_weights.size() || firstGroup.size() > bestGroupSizeSoFar) {
        return; 
    }

    // Another pruning condition 
    if(package_weights[pos] + getGroupWeight(firstGroup) <= targetWeight) {
        std::vector<int> firstGroupCopy = firstGroup;
        firstGroupCopy.push_back(package_weights[pos]);
        findFirstGroup(package_weights, pos + 1, firstGroupCopy);
    }
    findFirstGroup(package_weights, pos + 1, firstGroup);
    
}

int main() {
    std::string usrInput{""};

    std::cout << "For Part 2 (4 Groups) answer, press 'y'. Any other input gives Part 1 (3 Groups) answer: ";
    std::cin >> usrInput;

    if(usrInput == "y") {
        part2 = true;
    } 

    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }
    
    std::string line{}; // Holds the line read from the file

    std::vector<int> package_weights{};

    // Reading line by line
    while (std::getline(file, line)) { 
        package_weights.push_back(std::stoi(line)); 

    }

    file.close();

    // Calculating Target Weight of All Groups
    for(const int& weight : package_weights) {
        targetWeight += weight;
    }


    // Get target weight
    // For part 2 with 4 groups, targetWeight for our first group is instead 1/4. 
    if(part2) {
        targetWeight /= 4;
    } else {
        targetWeight /= 3;
    }
    

    std::vector<int> firstGroup{};

    findFirstGroup(package_weights, 0, firstGroup);

    std::cout << "QE of Ideal Configuration: " << bestQESoFar;

   
    return 0;

}