#include <cctype> // for isdigit
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

// split the input file into two blocks divided by an empty line
std::pair<std::vector<std::string>, std::vector<std::string>>
split_input(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file " + filename);
    }

    std::vector<std::string> block1;
    std::vector<std::string> block2;
    std::string line;
    bool empty_line = false;
    while (std::getline(file, line)) {
        if (line.empty()) {
            empty_line = true;
            continue;
        }
        if (!empty_line) {
            block1.push_back(line);
        } else {
            block2.push_back(line);
        }
    }
    return {block1, block2};
}

// function that splits the block1 into num_left and num_right divided with a |
// with regex and return both numbers
std::pair<int, int> split_numbers(const std::string &line) {
    std::regex re(R"((\d+)\|(\d+))");
    std::smatch match;
    if (std::regex_search(line, match, re)) {
        return {std::stoi(match[1]), std::stoi(match[2])};
    } else {
        throw std::runtime_error("Error: Unable to parse numbers from line " +
                                 line);
    }
}

// process the second block as a vector of vectors of int and return the vector
std::vector<std::vector<int>>
process_block2(const std::vector<std::string> &block2) {
    std::vector<std::vector<int>> result;
    for (const std::string &line : block2) {
        std::vector<int> row;
        std::string num;
        for (char c : line) {
            if (std::isdigit(c)) {
                num += c;
            } else if (!num.empty()) {
                row.push_back(std::stoi(num));
                num.clear();
            }
        }
        if (!num.empty()) {
            row.push_back(std::stoi(num));
        }
        result.push_back(row);
    }
    return result;
}

// get all the values of the map corresponding to the key and return them in a
// vector
std::vector<int> get_values(std::map<int, std::vector<int>> &before, int key) {
    if (before.find(key) == before.end()) {
        return {};
    }
    return before[key];
}

int main() {
    // Split the input file
    auto [block1_raw, block2_raw] = split_input("in.txt");

    // create a dictionary of the block1 in which the key is the num_left and
    // the values are the num_right if the key is already in the map, the value
    // is adding to the vector of values
    std::map<int, std::vector<int>> before;
    for (const std::string &line : block1_raw) {
        auto [num_left, num_right] = split_numbers(line);
        before[num_left].push_back(num_right);
    }

    // process the block2 as a vector of int
    std::vector<std::vector<int>> block2 = process_block2(block2_raw);

    // iterate over the block2 and check if any number in block 2 is against the
    // map of block1
    int sum = 0;
    // iterate over the block2 and check if any number in block 2 is against the
    // map of block1
    for (int i = 0; i < block2.size(); i++) {
        bool enabled = true;
        for (int j = 0; j < block2[i].size(); j++) {
            for (int k = j + 1; k < block2[i].size(); k++) {
                // get the values of the map corresponding to the key and return
                // them in a vector
                std::vector<int> values = get_values(before, block2[i][k]);

                for (int l = 0; l < values.size(); l++) {
                    // if the values are not empty, check if the values are
                    // equal to the block2[i][k] and if they are, set enabled to
                    // false
                    if (values[l] == block2[i][j]) {
                        enabled = false;
                        break;
                    }
                }
            }
        }
        if (enabled) {
            // add the middle number of the sequence to the sum
            sum += block2[i][block2[i].size() / 2];
        }
    }
    std::cout << sum << std::endl;
}
