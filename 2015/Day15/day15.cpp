#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

// Struct that holds information for Ingredients
struct IngredientInformation {
    int capacity;
    int durability;
    int flavor;
    int texture;
    int calories;
};


int num_ingredients{}; // Global variable for number of ingredients

int calculatePropertyTotal(const std::vector<IngredientInformation>& ingredients, const std::vector<int>& runningIngredientQuantity) {

    int capacity_total{0};
    int durability_total{0};
    int flavor_total{0};
    int texture_total{0};
    int calories_total{0};

    // Finding  the sums of all properties
    for(size_t i{0}; i < ingredients.size(); i++) {
        capacity_total += runningIngredientQuantity[i] * (ingredients[i].capacity);
        durability_total += runningIngredientQuantity[i] * (ingredients[i].durability);
        flavor_total += runningIngredientQuantity[i] * (ingredients[i].flavor);
        texture_total += runningIngredientQuantity[i] * (ingredients[i].texture);
        calories_total += runningIngredientQuantity[i] * (ingredients[i].calories);

    }
    
    // Returning a score. Negative totals are clamped to 0.
    // Part 2:  Non-500 calorie cookies are clamped to a property total of -1, 
    // representing an invalid/lowest score
    return (calories_total == 500) ? std::max(capacity_total, 0) * std::max(durability_total, 0) * std::max(flavor_total, 0) * std::max(texture_total, 0) : -1;
}


int findMaximumIngredient(const std::vector<IngredientInformation>& ingredients, int ingredientNum, int teaspoonsLeft, std::vector<int>& runningIngredientQuantity) {

    // Base Case
    if(ingredientNum == num_ingredients - 1) {
        // For the last ingredient, teaspoonsLeft is the only option
        runningIngredientQuantity.push_back(teaspoonsLeft);
        int result{calculatePropertyTotal(ingredients, runningIngredientQuantity)};
        runningIngredientQuantity.pop_back(); // backtracking
        return result;
    }

    int maximum_total{0};

    // Recursively choosing an available teaspoon amount for the current ingredient
    // then backtracking for other combinations
    for(int i{0}; i <= teaspoonsLeft; i++) {
        runningIngredientQuantity.push_back(i);
        maximum_total = std::max(maximum_total, findMaximumIngredient(ingredients, ingredientNum + 1, teaspoonsLeft - i, runningIngredientQuantity));
        runningIngredientQuantity.pop_back(); // backtracking
    }

    return maximum_total;

}



int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::regex pattern{"(\\w+): capacity (-?\\d+), durability (-?\\d+), flavor (-?\\d+), texture (-?\\d+), calories (-?\\d+)"};


    std::smatch match; // Holds matching groups in pattern 
    std::string line{}; // Holds the line read from the file

    std::vector<IngredientInformation> ingredients{};

    // Reading in line by line
    while (std::getline(file, line)) {

        // Storing data in the ingredients vector
        if (std::regex_search(line, match, pattern)) {
            ingredients.push_back({std::stoi(match[2].str()), std::stoi(match[3].str()), std::stoi(match[4].str()), std::stoi(match[5].str()), std::stoi(match[6].str())});
        }       
    }

    file.close();
    
    num_ingredients = int(ingredients.size());

    std::vector<int> runningIngredientQuantityStart{}; // Used because lname is 
    // needed for reference

    int ingredient_teaspoons{100}; // Number of total teaspoons

    std::cout << "Highest Scoring Cookie: " <<  findMaximumIngredient(ingredients, 0, ingredient_teaspoons, runningIngredientQuantityStart);
  
    return 0;

}