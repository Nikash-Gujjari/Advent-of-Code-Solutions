#include <iostream>
#include <fstream>
#include "json.hpp"  // downloaded json.hpp

using json = nlohmann::json;

// Recursive function to count numbers
void traverse_json(const json& j, int& sum) {
    if (j.is_object()) {
        for (auto& [key, val] : j.items()) { // Checking for Red in object 
                                            // and then ignoring children if found
            if(val == "red") {
                return;
            }
        }
        for (auto& [key, val] : j.items()) {
            traverse_json(val, sum);  // recurse into object values
        }
    } else if (j.is_array()) {
        for (auto& element : j) {
            traverse_json(element, sum);  // recurse into array elements
        }
    } else if (j.is_number()) {
        sum += j.get<int>();  // Summing all numbers
    }
}

int main() {
    std::ifstream file("input.json");  // your JSON file
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    json j;
    file >> j;  // parse JSON from file

    int sum{0};
    traverse_json(j, sum);  // count all numbers

    std::cout << "Sum of numbers in JSON: " << sum << "\n";

    return 0;
}