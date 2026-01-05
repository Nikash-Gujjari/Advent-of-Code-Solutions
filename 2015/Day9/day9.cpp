#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>

// Finds a Minimum Hamiltonian Path (a path [not cycle] that goes through all graph nodes exactly once) 
int hamiltonianPathMin(const std::unordered_map<std::string,
                  std::unordered_map<std::string, int>> &distGraph)
{
    // ---- Convert city names into indices ----
    std::vector<std::string> cities{};
    cities.reserve(distGraph.size());
    for (auto &p : distGraph)
        cities.push_back(p.first);

    int n = cities.size();
    std::unordered_map<std::string, int> idx{};
    for (int i = 0; i < n; i++)
        idx[cities[i]] = i;

    // ---- Build distance matrix W[i][j] ----
    const int INF = 1e9;
    std::vector<std::vector<int>> W(n, std::vector<int>(n, INF));

    for (int i = 0; i < n; i++) {
        W[i][i] = 0;
    }
    for (auto &a : distGraph) {
        for (auto &b : a.second) {
            int i{idx[a.first]};
            int j{idx[b.first]};
            W[i][j] = b.second;
        }
    }

    // ---- DP: Held–Karp ----
    int FULL = 1 << n; // Maintaining the visited number of cities through a mask, in which each position
    // reperesents whether a city has been visited (1 means visited, 0 means not). The FULL represents
    // that every city is visited, so all bits in the mask are 1

    // dp[mask][i] = min cost ending at i with visited-set = mask
    std::vector<std::vector<int>> dp(FULL, std::vector<int>(n, INF));

    // Start at any city (could be altered to start at a particular city or some subset of cities)
    for (int i = 0; i < n; i++) {
        dp[1 << i][i] = 0;
    }

    for (int mask = 0; mask < FULL; mask++) {
        for (int i = 0; i < n; i++) {

            // Check if the city is not visited (so not a '1' in the mask)
            if (!(mask & (1 << i))) {
                continue;
            }

            int cost{dp[mask][i]};

            // Check if there is a path (if not the cost is INF) 
            if (cost == INF) {
                continue;
            }

            for (int j = 0; j < n; j++) {
                if (mask & (1 << j)) continue; // already visited, so skip
                int nextMask{mask | (1 << j)}; // Create a new mask that represents visiting city 'j'
                dp[nextMask][j] =
                    std::min(dp[nextMask][j], cost + W[i][j]);
            }
        }
    }

    // ---- Find best ENDING CITY: no return to start ----
    int best = INF;
    for (int i = 1; i < n; i++) {
       best = std::min(best, dp[FULL - 1][i]);
    }

    return best;
}

// Part 2: Finding the Maximum Hamiltonian Path
int hamiltonianPathMax(const std::unordered_map<std::string,
                       std::unordered_map<std::string, int>> &distGraph)
{
    // ---- Convert city names into indices ----
    std::vector<std::string> cities{};
    for (auto &p : distGraph)
        cities.push_back(p.first);

    int n = cities.size();
    std::unordered_map<std::string, int> idx{};
    for (int i = 0; i < n; i++)
        idx[cities[i]] = i;

    // ---- Build distance matrix W[i][j] ----
    const int NEG_INF = -1e9;
    const int INF = 1e9;
    std::vector<std::vector<int>> W(n, std::vector<int>(n, NEG_INF));

    for (int i = 0; i < n; i++)
        W[i][i] = 0;

    for (auto &a : distGraph) {
        for (auto &b : a.second) {
            int i = idx[a.first];
            int j = idx[b.first];
            W[i][j] = b.second;
        }
    }

    // ---- DP: Held–Karp for Longest Hamiltonian Path ----
    int FULL = 1 << n;

    // dp[mask][i] = max cost ending at i with visited-set = mask
    std::vector<std::vector<int>> dp(FULL, std::vector<int>(n, NEG_INF));

    // Allow starting at ANY city
    for (int i = 0; i < n; i++)
        dp[1 << i][i] = 0;

    for (int mask = 0; mask < FULL; mask++) {
        for (int i = 0; i < n; i++) {
            if (!(mask & (1 << i))) continue;

            int cost = dp[mask][i];
            if (cost == NEG_INF) continue;

            for (int j = 0; j < n; j++) {
                if (mask & (1 << j)) continue; // already visited
                int nextMask = mask | (1 << j);

                // Checking if route exists. Different from other method because we set 
                // invalid paths to NEG_INF
                if (W[i][j] != NEG_INF) {
                    dp[nextMask][j] =
                        std::max(dp[nextMask][j], cost + W[i][j]);
                }
            }
        }
    }

    // ---- Find best ENDING CITY: no return to start ----
    int best = NEG_INF;
    for (int i = 0; i < n; i++) {
        best = std::max(best, dp[FULL - 1][i]);
    }

    return best;
}

int main() {
    std::ifstream file("input.txt"); // Open the file

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string line{}; // Holds the line read from the file

    std::unordered_map<std::string, std::unordered_map<std::string, int>> distGraph{}; // Holds a Graph representation of 
    // the distances, in which the distances between city "A" and city "B" is distGraph["A"]["B"]


    std::regex pattern{
        "(\\S+) to (\\S+) = (\\d+)"
    };
    std::smatch match; // Holds matching groups in pattern 


    // Reading in line by line
    while (std::getline(file, line)) {

        if (std::regex_search(line, match, pattern)) {
            distGraph[match[1].str()][match[2].str()] = std::stoi(match[3].str()); 
            distGraph[match[2].str()][match[1].str()] = std::stoi(match[3].str()); // This is done to ensure a fully inputted graph
        }       
    }

   // Using a Dynamic Programming Solution for finding the minimum and maximum distance Hamiltonian Path 

    std::cout << "Distance of Shortest Route: " << hamiltonianPathMin(distGraph) << "\n";
    std::cout << "Distance of Longest Route: " << hamiltonianPathMax(distGraph);

    file.close();


    return 0;

}