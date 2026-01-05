#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>


// Represents a hero's Spell
struct Spell {
    std::string name;
    int cost;
};

// Represents effects of spells
struct Effect {
    std::string name;
    int duration;
};

// Represents the hero and boss
struct Character {
    int hit_points;
    int damage;
    int armor;
    int mana;
};

// Possible Effects
Effect shieldEffect{"Shield", 6};
Effect poisonEffect{"Poison", 6};
Effect rechargeEffect{"Recharge", 5};

// Spell Definitions
std::vector<Spell> castableSpells {
    Spell{"Magic Missile", 53},
    Spell{"Drain", 73},
    Spell{"Shield", 113},
    Spell{"Poison", 173},
    Spell{"Recharge", 229}
};

// Keeps track of the CHEAPEST_SPELL cost so that if the hero's mana is below it on their turn
// we can end the recursion early
const int CHEAPEST_SPELL =
    std::min_element(castableSpells.begin(), castableSpells.end(),
                     [](const Spell& a, const Spell& b) {
                         return a.cost < b.cost;
                     })->cost;


int bestSoFar{INT_MAX}; // Accumulator variable that dynamically keeps track of cheapest mana to win. At the end it will hold the solution

// A recursive function that process a turn of play and recursively continue the game if possible
void processTurn(Character hero, Character boss, std::vector<Effect> activeEffects, bool heroTurn, int manaUsed) {

    // Quitting Early If We Already Found A Better Solution
    if(manaUsed >= bestSoFar) {
        return;
    }

    // Part 2: Hard difficulty reduces player health by 1 on player turn. Comment this if-statement for part 1 solution
    if(heroTurn) {
        hero.hit_points--;
    }

    // Checking if player lost
    if(hero.hit_points <= 0) {
        return;
    }

    // Used for the the armor in damage calculation against a hero. It will be calculated as base + shield effect (if present)
    int effective_hero_armor{hero.armor}; 

    // Processing Effects
    for(Effect& effect : activeEffects) {
        if(effect.name == "Poison") {
            boss.hit_points -= 3;
        } else if(effect.name == "Recharge") {
            hero.mana += 101;
        } else if(effect.name == "Shield") {
            effective_hero_armor += 7; // Updating effective armor here
        }
        effect.duration--;
    }

    // Seeing if either character won. If the Hero won (boss hit points goes to 0), we see if the mana used was cheaper than
    // our best so far. Otherwise if the Boss wins, we ignore it because we only care about winning sequences. 
    if(boss.hit_points <= 0) {
        bestSoFar = std::min(bestSoFar, manaUsed);
        return;
    }




    std::unordered_set<std::string> notUsableSpells{}; // Keeps tracks of spells that can not be casted because their effects are still active

    // Removing Any Efects that Hit a Duration of 0 and tracking spells that could not be cast
    for(auto it{activeEffects.begin()}; it != activeEffects.end();) {
        if(it->duration == 0) {
            it = activeEffects.erase(it); // After deletion the erase() function will return an iterator that pointso the next element
        } else {
            notUsableSpells.insert(it->name); 
            it++; // If erase() is not called, we update the iterator manually to go to the next element
        }
    }

    // Boss turn to damage Hero
    if(!heroTurn) {
        
        hero.hit_points -= std::max(1, boss.damage - effective_hero_armor);
        processTurn(hero, boss, activeEffects, true, manaUsed); // Going to the (next) hero's turn

    } else { // Hero turn to cast spell

        // Losing because the hero has no mana
        if(hero.mana < CHEAPEST_SPELL) {
            return;
        }
        // Choosing a Hero Spell to Cast
        for(Spell& spell : castableSpells) {
            // Checking that the hero can validly cast the spell: they have enough mana and if it has a spell with a persistent
            // effect then that persistent effect is not present
            if(hero.mana >=  spell.cost && notUsableSpells.count(spell.name) == 0) {
                // Copying the Hero, Boss, and activeEffects for each recursive "branch"
                Character heroCopy = hero;
                Character bossCopy = boss;
                std::vector<Effect> activeEffectsCopy = activeEffects;
                // Applying a spell's damage and effects based on its game definition
                if(spell.name == "Magic Missile") {
                    bossCopy.hit_points -= 4;  
                } else if(spell.name == "Drain") {
                    bossCopy.hit_points -= 2;
                    heroCopy.hit_points += 2;
                } else if(spell.name == "Shield") {
                    activeEffectsCopy.push_back(shieldEffect);
                } else if(spell.name == "Poison") {
                    activeEffectsCopy.push_back(poisonEffect);
                } else if(spell.name == "Recharge") {
                    activeEffectsCopy.push_back(rechargeEffect);
                }
                heroCopy.mana -= spell.cost; // paying spell cost
                processTurn(heroCopy, bossCopy, activeEffectsCopy, false, manaUsed + spell.cost); // Going to next (boss's) turn               
            }
        }
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

    Character boss{}; // Struct to hold boss information
    
    // Unused fields defaulted to 0
    boss.armor = 0;
    boss.mana = 0; 

    // Reading line by line
    while (std::getline(file, line)) { 


        std::regex pattern{"(Hit Points|Damage): (\\d+)"}; // Pattern for extracting data

        if (std::regex_search(line, match, pattern)) {
            if(match[1].str() == "Hit Points") {
                boss.hit_points = std::stoi(match[2]);
            } else if(match[1].str() == "Damage") {
                boss.damage = std::stoi(match[2]);   
            }
        }   

    }
    file.close();

    // Creating the player character and starting the recursive method
    processTurn(Character{50, 0, 0, 500}, boss, std::vector<Effect>{}, true, 0); 

    std::cout << "Least Amount of Mana You Can Spend: " << bestSoFar;

    return 0;

}