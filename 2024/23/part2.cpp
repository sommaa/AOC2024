#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

std::vector<std::pair<std::string, std::string>> read_input() {
    std::vector<std::pair<std::string, std::string>> input;
    std::string line;
    while (std::getline(std::cin, line)) {
        size_t pos = line.find('-');
        input.push_back({line.substr(0, pos), line.substr(pos + 1)});
    }
    return input;
}

std::set<std::vector<std::string>> sets;

void search_(
    const std::string &node, std::vector<std::string> required,
    const std::unordered_map<std::string, std::vector<std::string>> &graph) {
    std::sort(required.begin(), required.end());
    if (sets.find(required) != sets.end()) {
        return;
    }
    sets.insert(required);

    for (const auto &neighbor : graph.at(node)) {
        if (std::find(required.begin(), required.end(), neighbor) !=
            required.end()) {
            continue;
        }

        bool all_connected = true;
        for (const auto &required_node : required) {
            const auto &neighbors = graph.at(neighbor);
            if (std::find(neighbors.begin(), neighbors.end(), required_node) ==
                neighbors.end()) {
                all_connected = false;
                break;
            }
        }
        if (!all_connected) {
            continue;
        }

        auto new_required = required;
        new_required.push_back(neighbor);
        search_(neighbor, std::move(new_required), graph);
    }
}

int main() {
    auto input = read_input();
    std::unordered_map<std::string, std::vector<std::string>> graph;

    for (const auto &[a, b] : input) {
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    for (const auto &[key, _] : graph) {
        search_(key, {key}, graph);
    }

    // find the sets with the maximum size and print the pc names
    size_t max_size = 0;
    for (const auto &set : sets) {
        if (set.size() > max_size) {
            max_size = set.size();
        }
    }

    for (const auto &set : sets) {
        if (set.size() == max_size) {
            for (const auto &pc : set) {
                std::cout << pc << ',';
            }
            std::cout << '\n';
        }
    }
    return 0;
}
