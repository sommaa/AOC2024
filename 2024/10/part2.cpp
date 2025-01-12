#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Import the input file as a matrix of digits
std::vector<std::vector<int>> import_input(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << "Error: Could not open file " << file_path << std::endl;
        exit(1);
    }
    std::vector<std::vector<int>> input;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines
        std::vector<int> row;
        for (char ch : line) {
            if (std::isdigit(ch)) { // Check if the character is a digit
                row.push_back(ch - '0'); // Convert char to int
            }
        }
        input.push_back(row);
    }
    return input;
}

// Recursive function to check neighbors of a cell
int check_neighbors(std::vector<std::vector<int>>& input, int i, int j) {
    // if is a 9 return 1 
    if (input[i][j] == 9) {
        return 1;
    }
    // check up, down, left, right 
    int result = 0;
    std::vector<std::vector<int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (auto& dir : directions) {
        int nx = i + dir[0];
        int ny = j + dir[1];
        if (nx >= 0 && nx < input.size() && ny >= 0 && ny < input[0].size()) {
            if (input[nx][ny] == 1 + input[i][j]) {
                result += check_neighbors(input, nx, ny);
            }
        }
    }
    return result;
}

int main() {
    // Read the input
    std::vector<std::vector<int>> input = import_input("in.txt");

    // Initialize result
    int result = 0;

    // Iterate over the matrix to find all zeros
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            if (input[i][j] == 0) {
                // Find reachable 9s from this 0
                result += check_neighbors(input, i, j);
            }
        }
    }
    std::cout << result << std::endl;
    return 0;
}
