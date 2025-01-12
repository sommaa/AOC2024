#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

void parseInput(
    std::vector<char> &directions,
    std::map<std::string, std::pair<std::string, std::string>> &nodes) {
    std::string line;

    // Read the first line for directions
    std::getline(std::cin, line);
    for (char c : line) {
        if (c == 'R' || c == 'L') {
            directions.push_back(c);
        }
    }

    // Read subsequent lines for node mappings
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string node, equals, leftChild, rightChild;

        // Parse lines like: AAA = (BBB, CCC)
        if (std::getline(iss, node, ' ') && iss >> equals && equals == "=" &&
            iss >> std::ws && iss.get() == '(' &&
            std::getline(iss, leftChild, ',') &&
            std::getline(iss, rightChild, ')')) {

            // Trim whitespace from children
            leftChild.erase(leftChild.find_last_not_of(" ") + 1);
            rightChild.erase(0, rightChild.find_first_not_of(" "));

            // Store node mapping
            nodes[node] = {leftChild, rightChild};
        }
    }
}

std::pair<std::string, int> calculate_steps(
    const std::string &start, std::vector<std::string> &end,
    const std::map<std::string, std::pair<std::string, std::string>> &nodes,
    const std::vector<char> &directions) {
    std::string local_node = start;
    int i = 0;
    int steps = 0;

    while (std::find(end.begin(), end.end(), local_node) == end.end()) {
        if (nodes.find(local_node) == nodes.end()) {
            break; // Exit if the node doesn't exist in the map
        }

        const auto &children = nodes.at(local_node);
        if (directions[i] == 'R') {
            local_node = children.second;
        } else {
            local_node = children.first;
        }
        i = (i + 1) % directions.size();
        steps++;
    }
    return {local_node, steps};
}

int main() {
    std::vector<char> directions;
    std::map<std::string, std::pair<std::string, std::string>> nodes;

    parseInput(directions, nodes);

    std::vector<std::pair<std::string, int>> steps_per_node;
    std::vector<std::string> local_nodes;
    std::vector<std::string> ending_nodes;

    // Find starting nodes (ending in 'A')
    for (const auto &node : nodes) {
        if (!node.first.empty() && node.first.back() == 'A') {
            local_nodes.push_back(node.first);
        }
    }

    // Find ending nodes (ending in 'Z')
    for (const auto &node : nodes) {
        if (!node.first.empty() && node.first.back() == 'Z') {
            ending_nodes.push_back(node.first);
        }
    }

    for (const auto &local_node : local_nodes) {
        steps_per_node.push_back(
            calculate_steps(local_node, ending_nodes, nodes, directions));
    }

    // print the steps
    for (const auto &step : steps_per_node) {
        std::cout << step.first << " " << step.second << std::endl;
    }

    // Calculate the minimum common multiple of the steps
    long long min_steps = 0;
    for (const auto &step : steps_per_node) {
        if (min_steps == 0) {
            min_steps = step.second;
        } else {
            min_steps = std::lcm(min_steps, step.second);
        }
    }
    std::cout << "lcm: " << min_steps << std::endl;
    return 0;
}
