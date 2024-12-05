#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype> // for isdigit
#include <stdexcept>
#include <regex>

// split the input file into two blocks divided by an empty line 
std::pair<std::vector<std::string>, std::vector<std::string>> split_input(const std::string& filename) {
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

// function that splits the block1 into num_left and num_right divided with a | with regex and return both numbers 
std::pair<int, int> split_numbers(const std::string& line) {
    std::regex re(R"((\d+)\|(\d+))");
    std::smatch match;
    if (std::regex_search(line, match, re)) {
        return {std::stoi(match[1]), std::stoi(match[2])};
    } else {
        throw std::runtime_error("Error: Unable to parse numbers from line " + line);
    }
}

// process the second block as a vector of vectors of int and return the vector 
std::vector<std::vector<int>> process_block2(const std::vector<std::string>& block2) {
    std::vector<std::vector<int>> result;
    for (const std::string& line : block2) {
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


// get all the values of the map corresponding to the key and return them in a vector  
std::vector<int> get_values(std::map<int, std::vector<int>>& before, int key) {
    if (before.find(key) == before.end()) {
        return {};
    }
    return before[key];
}

// valid function that returns a boolean value  
bool valid(std::map<int, std::vector<int>>& before, std::vector<int>& block2_line) {
    bool enabled = true;
    for (int j = 0; j < block2_line.size(); j++) {
         for (int k = j + 1; k < block2_line.size(); k++) {
             // get the values of the map corresponding to the key and return them in a vector 
             std::vector<int> values = get_values(before, block2_line[k]);
             
             for (int l = 0; l < values.size(); l++) {
             // if the values are not empty, check if the values are equal to the block2[i][k] and if they are, set enabled to false
                 if (values[l] == block2_line[j]) {
                     enabled = false;
                     break;
                 }
             }
         }
    }
    return enabled;
}

int main() {
    // Split the input file
    auto [block1_raw, block2_raw] = split_input("in.txt");
    
    // create a dictionary of the block1 in which the key is the num_left and the values are the num_right 
    // if the key is already in the map, the value is adding to the vector of values 
    std::map<int, std::vector<int>> before;
    for (const std::string& line : block1_raw) {
        auto [num_left, num_right] = split_numbers(line);
        before[num_left].push_back(num_right);
    }

    // process the block2 as a vector of int    
    std::vector<std::vector<int>> block2 = process_block2(block2_raw);
    
    // iterate over the block2 and check if any number in block 2 is against the map of block1
    int sum = 0;
    for (int i = 0; i < block2.size(); i++) {
        bool enabled = valid(before, block2[i]);
        bool enabled_slice;

        if (!enabled) {
            while (!enabled) {
                for (int j = block2[i].size() - 1; j > 0; j--) {
                    // check if from position j to the end of the block2[i] is correct 
                    std::vector<int> sliced_copy(block2[i].begin() + j - 1, block2[i].end());
                    enabled_slice = valid(before, sliced_copy);

                    if (!enabled_slice) {
                        // swap the values of the block2[i][j] and block2[i][j + 1]
                        std::swap(block2[i][j], block2[i][j - 1]);
                    }
                }
                enabled = valid(before, block2[i]);
            }
            sum += block2[i][block2[i].size() / 2];
        }
    }
    std::cout << sum << std::endl;
}
