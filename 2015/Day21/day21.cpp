#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cmath>

// Represents Items
struct Item {
    int cost;
    int damage;
    int armor;
};

// Represents the hero and boss
struct Character {
    int hit_points;
    int damage;
    int armor;
};

// Calculates all equippable subsets of rings 
std::vector<std::vector<Item>> ringSubSets(const std::vector<Item>& rings) {
    std::vector<std::vector<Item>> result;

    result.push_back({});

    // Single ring subsets
    for (const auto& ring : rings) {
        result.push_back({ring});
    }

    // Tow ring subsets
    for (size_t i = 0; i < rings.size(); ++i) {
        for (size_t j = i + 1; j < rings.size(); ++j) {
            result.push_back({rings[i], rings[j]});
        }
    }

    return result;
}

// Calculates winning a fight by seeing if the hero takes less than or same (since the hero goes first) number of turns the hero needs to defeat the boss
// as the boss would need to defeat the hero
bool winFight(Character hero, Character boss) {
    int heroTurns{int(std::ceil(double(boss.hit_points) / double(std::max(1, hero.damage - boss.armor))))};
    int bossTurns{int(std::ceil(double(hero.hit_points) / double(std::max(1, boss.damage - hero.armor))))};

    return heroTurns <= bossTurns;
}

int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }
    
    std::smatch match{}; // Holds matching groups in pattern 
    std::string line{}; // Holds the line read from the file

    Character boss{}; // Struct to hold boss information

    // Reading line by line
    while (std::getline(file, line)) { 


        std::regex pattern{"(Hit Points|Damage|Armor): (\\d+)"}; // Pattern for extracting string conversions

        if (std::regex_search(line, match, pattern)) {
            if(match[1].str() == "Hit Points") {
                boss.hit_points = std::stoi(match[2]);
            } else if(match[1].str() == "Damage") {
                boss.damage = std::stoi(match[2]);   
            } else if(match[1].str() == "Armor") {
                boss.armor = std::stoi(match[2]);
            }
        }   

    }

    file.close();

    // Shopkeep Items

    std::vector<Item> weapons{
        Item{8, 4, 0},
        Item{10, 5, 0},
        Item{25, 6, 0},
        Item{40, 7, 0},
        Item{74, 8, 0}
    };

    std::vector<Item> armor{
        Item{0, 0, 0}, // No Armor Choice
        Item{13, 0, 1},
        Item{31, 0, 2},
        Item{53, 0, 3},
        Item{75, 0, 4},
        Item{102, 0, 5}
    };

    std::vector<Item> rings{
        Item{25, 1, 0},
        Item{50, 2, 0},
        Item{100, 3, 0},
        Item{20, 0, 1},
        Item{40, 0, 2},
        Item{80, 0, 3}
    };

    std::vector<std::vector<Item>> ringSubSet{ringSubSets(rings)}; // All combinations of equippable rings (<= 2 distinct rings, for which order does not matter).


    int minCost{INT_MAX};
    int cost{0};
    int maxCost{INT_MIN}; // Part 2

    // With an empty Item{0, 0, 0} and {} for armor and ringSubSet, respectively, we can use this loop structure and still calculate the choice of not using rings and/or amor. 
    for(const auto& weapon : weapons) {
        for(const auto& armor_piece: armor) {
            for(const auto& ringSets : ringSubSet) {
                // These variables holds stats for all rings in a subset
                int ring_damage{0};
                int ring_armor{0};
                int ring_cost{0};
                for(const auto& ring : ringSets) {
                    ring_damage += ring.damage;
                    ring_armor += ring.armor;
                    ring_cost += ring.cost;
                }
                Character player{100, weapon.damage + armor_piece.damage + ring_damage, weapon.armor + armor_piece.armor + ring_armor};
                cost = weapon.cost + armor_piece.cost + ring_cost;

                if (winFight(player, boss)) {
                    minCost = std::min(cost, minCost);
                } else {
                    maxCost = std::max(cost, maxCost);
                }
            }

        }
    }

    std::cout << "Minimum Cost For Win: " << minCost << std::endl ;
    std::cout << "Maximum Cost For Lose: " << maxCost;
    
    return 0;

}