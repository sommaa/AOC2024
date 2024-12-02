#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

// import the input file as a vector of strings
std::vector<std::string> import_input(std::string filename) {
    std::ifstream file(filename);
    std::vector<std::string> input;
    std::string line;
    while (std::getline(file, line)) {
        input.push_back(line);
    }
    return input;
}

// split the string of each line into the two numbers
std::pair<int, int> split_string(std::string line) {
    int pos = line.find("  ");
    int left = std::stoi(line.substr(0, pos));
    int right = std::stoi(line.substr(pos + 1));
    return std::make_pair(left, right);
}


int main() {
    std::vector<std::string> input = import_input("in.txt");
    std::vector<int> col1;
    std::vector<int> col2;
    for (std::string line : input) {
        std::pair<int, int> nums = split_string(line);
        col1.push_back(nums.first);
        col2.push_back(nums.second);
    }

    std::map<int, int> col1_map;
    int sumOfOccurrences = 0;
    for (int i = 0; i < col1.size(); i++) {
        // if the element i of col1 is not in the map, add to the map with a value corresponding to number of occurrences in col2
        if (col1_map.find(col1[i]) == col1_map.end()) {
            col1_map[col1[i]] = std::count(col2.begin(), col2.end(), col1[i]);
        } else {
            // if the element i of col1 is in the map, increment the value corresponding to number of occurrences in col2
            col1_map[col1[i]] += std::count(col2.begin(), col2.end(), col1[i]);
        }
        // multiply the number of occurrences of the element i of col1 in col2 by the element i of col1 and add to the sum 
        sumOfOccurrences += col1_map[col1[i]] * col1[i];
    }
    std::cout << sumOfOccurrences << std::endl;
    return 0;
}

