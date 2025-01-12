#include <iostream>
#include <string>
#include <vector>

std::vector<std::vector<char>> importData() {
    std::vector<std::vector<char>> data;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        data.push_back(row);
    }
    return data;
}

std::vector<std::pair<int, int>> adjacency_list = {
    {0, 1},   // right
    {1, 0},   // down
    {0, -1},  // left
    {-1, 0},  // up
    {1, 1},   // down-right
    {1, -1},  // down-left
    {-1, -1}, // up-left
    {-1, 1}   // up-right
};

bool isSymbol(char c) {
    return !isdigit(c) && c != '.';
}

bool hasAdjacentSymbol(const std::vector<std::vector<char>>& data, int i, int j) {
    for (const auto& adj : adjacency_list) {
        int newI = i + adj.first;
        int newJ = j + adj.second;
        
        if (newI >= 0 && newI < data.size() && 
            newJ >= 0 && newJ < data[newI].size() && 
            isSymbol(data[newI][newJ])) {
            return true;
        }
    }
    return false;
}

int main() {
    int sum = 0;
    std::vector<std::vector<char>> data = importData();
    
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            if (isdigit(data[i][j])) {
                std::string number;
                bool hasSymbol = false;
                
                // Get full number
                while (j < data[i].size() && isdigit(data[i][j])) {
                    number += data[i][j];
                    if (!hasSymbol && hasAdjacentSymbol(data, i, j)) {
                        hasSymbol = true;
                    }
                    j++;
                }
                
                if (hasSymbol) {
                    sum += std::stoi(number);
                }
                
                j--;
            }
        }
    }
    
    std::cout << "Sum: " << sum << std::endl;
    return 0;
}
