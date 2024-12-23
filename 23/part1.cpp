#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

// Import input file as string of the form computer1-computer2\n
std::vector<std::pair<std::string, std::string>> read_input() {
    std::vector<std::pair<std::string, std::string>> input;
    std::string line;
    while (std::getline(std::cin, line)) {
        size_t pos = line.find('-');
        input.push_back({line.substr(0, pos), line.substr(pos + 1)});
    }
    return input;
}

int main() {
    auto input = read_input();
    std::unordered_map<std::string, std::vector<std::string>> graph;
    int total_sets_with_t = 0;

    // Build the adjacency list
    for (const auto &[a, b] : input) {
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    // Use set of tuple<string, string, string> to store unique triangles
    std::set<std::tuple<std::string, std::string, std::string>> sets;

    // Find all triangles in the graph
    for (const auto &[key, _] : graph) {
        for (const auto &value_1 : graph[key]) {
            for (const auto &value_2 : graph[value_1]) {
                // Check if there's an edge back to the original vertex
                if (key != value_2 &&
                    std::find(graph[value_2].begin(), graph[value_2].end(),
                              key) != graph[value_2].end()) {

                    // Create sorted strings to ensure unique triangles
                    std::vector<std::string> triangle = {key, value_1, value_2};
                    std::sort(triangle.begin(), triangle.end());

                    // Create and insert the tuple
                    sets.insert(
                        std::make_tuple(triangle[0], triangle[1], triangle[2]));
                }
            }
        }
    }

    // Count the number of sets containing at least one of the computers starting with 't'
    for (const auto &[a, b, c] : sets) {
        if (a[0] == 't' || b[0] == 't' || c[0] == 't') {
            total_sets_with_t++;
        }
    }
  
    // Print the number of unique triangles
    std::cout << total_sets_with_t << std::endl;
    return 0;
}
