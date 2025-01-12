#include <iostream>
#include <map>
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

int main() {
    std::vector<char> directions;
    std::map<std::string, std::pair<std::string, std::string>> nodes;

    parseInput(directions, nodes);

    std::string local_node = "AAA";
    int i = 0;
    int steps = 0;
    while (local_node != "ZZZ") {
        std::cout << local_node << std::endl;
        std::pair<std::string, std::string> children = nodes[local_node];
        if (directions[i] == 'R') {
            local_node = children.second;
        } else {
            local_node = children.first;
        }
        i = (i + 1) % directions.size();
        steps++;
    }
    std::cout << steps << std::endl;
    return 0;
}
