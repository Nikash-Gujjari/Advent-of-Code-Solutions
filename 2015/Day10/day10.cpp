#include <iostream>
#include <string>

int main() {
    std::string input = "1113222113";

    for (int i = 0; i < 50; ++i) {
        std::string next;
        next.reserve(input.size() * 2); // Reserve to reduce reallocations

        char prev = input[0];
        int count = 1;

        for (size_t j = 1; j < input.size(); ++j) {
            if (input[j] == prev) {
                count++;
            } else {

                // Adding the frequency and number to the next iteration of input
                next += std::to_string(count);
                next += prev;
                prev = input[j];
                count = 1;
            }
        }

        // Add the last group (not considered by comparison within the for loop)
        next += std::to_string(count);
        next += prev;

        input.swap(next); // Efficiently replace input with next
    }

    std::cout << "Number of digits: " << input.size() << "\n";

    return 0;
}