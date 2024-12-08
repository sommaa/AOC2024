#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// import the input file line by line
std::vector<std::string> import_input(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file " + filename);
    }

    std::vector<std::string> block;
    std::string line;
    while (std::getline(file, line)) {
        block.push_back(line);
    }
    return block;
}

// fill map with the positions of the antennas
void fill_map(std::map<char, std::vector<std::pair<int, int>>> &map,
              std::vector<std::string> &block) {
    int rows = block.size();
    int cols = block[0].size();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char c = block[i][j];
            if (c != '.') {
                map[c].push_back(std::make_pair(i, j));
            }
        }
    }
}

int main() {
    // import the input file
    std::vector<std::string> block = import_input("in.txt");

    // create a map of char as key and 2 by position as value
    std::map<char, std::vector<std::pair<int, int>>> map;
    int rows = block.size();
    int cols = block[0].size();

    // iterate over the block and store the positions of the antennas
    fill_map(map, block);

    // create a bool matrix to store the anti-nodes positions
    std::vector<std::vector<bool>> anti_nodes(rows,
                                              std::vector<bool>(cols, false));

    // for each antenna, calculate the distance with sign to the other antennas
    // of the same type
    for (auto &it : map) {
        // c = antenna type
        char c = it.first;
        // positions = vector of positions of the antennas of type c
        std::vector<std::pair<int, int>> positions = it.second;
        for (int i = 0; i < positions.size(); i++) {
            for (int j = i + 1; j < positions.size(); j++) {
                // calculate the distance between the two antennas
                int distancex = positions[i].first - positions[j].first;
                int distancey = positions[i].second - positions[j].second;
                // add the anti-nodes to the matrix
                std::pair<int, int> anti_node1 =
                    std::make_pair(positions[i].first + distancex,
                                   positions[i].second + distancey);
                std::pair<int, int> anti_node2 =
                    std::make_pair(positions[j].first - distancex,
                                   positions[j].second - distancey);
                if (anti_node1.first >= 0 && anti_node1.first < rows &&
                    anti_node1.second >= 0 && anti_node1.second < cols) {
                    anti_nodes[anti_node1.first][anti_node1.second] = true;
                }
                if (anti_node2.first >= 0 && anti_node2.first < rows &&
                    anti_node2.second >= 0 && anti_node2.second < cols) {
                    anti_nodes[anti_node2.first][anti_node2.second] = true;
                }
            }
        }
    }
    // sum the bool matrix to get the number of anti-nodes in the block
    int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (anti_nodes[i][j]) {
                count++;
            }
        }
    }
    std::cout << count << std::endl;
    return 0;
}
