#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

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
    // sort the columns
    std::sort(col1.begin(), col1.end());
    std::sort(col2.begin(), col2.end());
    // sum the distances between the columns
    int sumOfDistances = 0;
    for (int i = 0; i < col1.size(); i++) {
        sumOfDistances += std::abs(col1[i] - col2[i]);
    }
    std::cout << sumOfDistances << std::endl;
    return 0;
}
