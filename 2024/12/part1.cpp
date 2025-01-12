#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

// from block of chars to vector of vectors of chars
std::vector<std::vector<char>> get_map(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return {}; // Return an empty vector
    }

    std::vector<std::vector<char>> block;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        block.push_back(row);
    }
    return block;
}

int main() {
    std::vector<std::vector<char>> map = get_map("in.txt");

    // create a set of seen coordinates x and y
    std::set<std::pair<int, int>> seen;

    // directions
    std::vector<std::pair<int, int>> dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    // rows and columns
    int rows = map.size();
    int cols = map[0].size();
    int result = 0;

    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            // if already seen, skip
            if (seen.find({x, y}) != seen.end()) {
                continue;
            }
            // create a deque of coordinates
            std::deque<std::pair<int, int>> q;
            q.push_back({x, y});
            int area = 0;
            int perimeter = 0;
            // while deque is not empty
            while (!q.empty()) {
                std::pair<int, int> coord = q.front();
                q.pop_front();
                if (seen.find(coord) != seen.end()) {
                    continue;
                }
                seen.insert(coord);
                area++;
                // for in dirs
                for (std::pair<int, int> dir : dirs) {
                    int next_x = coord.first + dir.first;
                    int next_y = coord.second + dir.second;
                    // if in bounds and character matches
                    if (next_x >= 0 && next_x < rows && next_y >= 0 &&
                        next_y < cols && map[next_x][next_y] == map[x][y]) {
                        q.push_back({next_x, next_y});
                    } else {
                        perimeter++;
                    }
                }
            }
            result += area * perimeter;
        }
    }
    std::cout << "Result: " << result << std::endl;
    return 0;
}
