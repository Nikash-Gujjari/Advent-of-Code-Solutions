#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <set>
#include <numeric>

// Custom data structure that is a map stable sorted by values
template<typename K, typename V>
class StableValueMap {
    struct Node {
        K key;
        V value;
        Node(const K& k, const V& v) : key(k), value(v) {}
    };

    struct Compare {
        bool operator()(const Node& a, const Node& b) const {
            if (a.value != b.value) return a.value > b.value;
            return a.key < b.key;
        }
    };

    using SetT = std::set<Node, Compare>;
    SetT sorted;
    std::unordered_map<K, typename SetT::iterator> index;

public:
    // Inserts a key-value pair into the map
    void insert(const K& key, const V& value) {
        auto it = index.find(key);
        if(it != index.end()) {
            sorted.erase(it->second);
            index.erase(it);
        }
        auto pos = sorted.insert(Node{key, value}).first;
        index.emplace(key, pos);
    }

    // Removes a key-value pair from the map
    void erase(const K& key) {
        auto it = index.find(key);
        if (it != index.end()) {
            sorted.erase(it->second);
            index.erase(it);
        }
    }

    // Returns a value associated with a key in the map
    const V* find(const K& key) const {
        auto it = index.find(key);
        if(it != index.end()) return &it->second->value;
        return nullptr;
    }

    // Empties the data structure
    void clear() {
        sorted.clear();
        index.clear();
    }

    // Returns an iterator
    auto begin() const { return sorted.begin(); }
    auto end() const { return sorted.end(); }
};

int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string line{}; // Holds the line read from the file
    std::smatch match{}; // Holds matching groups in pattern 

    StableValueMap<char, int> sortedMap{}; // Holds a map of letters to their frequency, sorted by their frequency for
    // each line of input

    std::vector<int> realRoomSectorIds{}; // Keeps track of the sector IDs of Real Rooms

    std::unordered_map<std::string, int> realroomNameToSectorId{}; // Part 2: Keep track of each Room Name's associated sector ID

    // Reading file input line by line
    while(std::getline(file, line)) {
        std::regex pattern{"^(\\S+)-(\\d+)\\[(\\w+)\\]$"}; // Pattern for extracting data
        if(std::regex_match(line, match, pattern)) {
            for(const char& letter : match[1].str()) {
                // Adding Non '-' characters to the frequency map
                if(letter != '-') {
                    if (const int* count = sortedMap.find(letter)) {
                        sortedMap.insert(letter, *count + 1);
                    } else {
                        sortedMap.insert(letter, 1);
                    }
                }
            }

            // Iterating through the checksum letters and seeing if the order they appear in matches the order
            // of our Map. Our map is iterated at the same time through an iterator object that we increment
            // at the same time as our range-based for loop
            auto it = sortedMap.begin(); 
            bool foundError{false};
            for(const char& checksumLetter : match[3].str()) {
                const auto& node = *it;
                if(node.key != checksumLetter) { // Breaking if there is a discrepancy early
                    foundError = true;
                    break;
                }
                ++it;
            }

            if(!foundError) {
                int roomSectorId{std::stoi(match[2].str())};
                realRoomSectorIds.push_back(roomSectorId); // If their was not a discrepancy with the checksum we 
                // take note of the Room's Sector ID for summation later on
                realroomNameToSectorId[match[1].str()] = roomSectorId;
            }

            sortedMap.clear(); // Resetting Map for next iteration

        }
    }

    // Part 1: Obtaining the sum of all real room name sector IDs
    int realRoomSectorIDSum{};
    for(const auto&[_, sectorId]: realroomNameToSectorId) {
        realRoomSectorIDSum += sectorId;
    }
    std::cout << "Sum of Real Room Sector IDs: " << realRoomSectorIDSum << std::endl;;

    // Part 2: Decrypting each real room name using its Sector ID for a shift cypher
    for(const auto&[roomName, sectorId] : realroomNameToSectorId) {
        std::string decryptedName{""};
        for(const char& letter: roomName) {
            if(letter == '-') {
                decryptedName.push_back(' ');
            } else {
                decryptedName.push_back(char(((int(letter - 'a') + (sectorId % 26)) % 26) + 'a'));
            }
            
        }
        std::cout << decryptedName << " , Sector ID: " << sectorId << std::endl;
    }

    // To find the right room and its sector ID, I piped the output of the executable to the Select-String PowerShell cmdlet: 
    // ./day4.exe | Select-String -Pattern 'pole'

    return 0;


}
