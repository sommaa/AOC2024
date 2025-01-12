#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>

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

bool isStar(char c) {
    return c == '*';
}

size_t hashStar(int i, int j) {
    // Hash function for star coordinates
    // Using Cantor pairing function
    return (i + j) * (i + j + 1) / 2 + j;
}

std::pair<int,int> hasAdjacentStar(const std::vector<std::vector<char>>& data, int i, int j) {
    for (const auto& adj : adjacency_list) {
        int newI = i + adj.first;
        int newJ = j + adj.second;
        
        if (newI >= 0 && newI < data.size() && 
            newJ >= 0 && newJ < data[newI].size() && 
            isStar(data[newI][newJ])) {
            return {newI, newJ};
        }
    }
    return {-1, -1};
}

int main() {
    int sum = 0;
    std::vector<std::vector<char>> data = importData();
    std::unordered_map<size_t, std::vector<int>> idStarNum;
    
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            if (isdigit(data[i][j])) {
                std::string number;
                bool hasStar = false;
                int starI = -1, starJ = -1;
                
                // Get full number
                while (j < data[i].size() && isdigit(data[i][j])) {
                    number += data[i][j];
                    auto [newStarI, newStarJ] = hasAdjacentStar(data, i, j);
                    if (!hasStar && newStarI != -1) {
                        starI = newStarI;
                        starJ = newStarJ;
                        hasStar = true;
                    }
                    j++;
                }
                
                if (hasStar) {
                    size_t starNum = hashStar(starI, starJ);
                    idStarNum[starNum].push_back(std::stoi(number));
                }
                
                j--;
            }
        }
    }
    
    for (const auto& [id, numbers] : idStarNum) {
        if (numbers.size() == 2) {
            sum += numbers[0] * numbers[1];
        }
    }
    
    std::cout << "Sum: " << sum << std::endl;
    return 0;
}
