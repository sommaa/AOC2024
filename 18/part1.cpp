#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// read a file that contains num,num\n
std::vector<std::pair<int, int>> read_file(const std::string &filename) {
    std::vector<std::pair<int, int>> result;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            int a, b;
            char comma;
            if (iss >> a >> comma >> b) {
                result.push_back({a, b});
            }
        }
    }
    return result;
}

void display_bfs(const std::vector<std::vector<std::string>> &grid, int rows,
                 int cols, std::queue<std::tuple<int, int, int>> q,
                 std::set<std::pair<int, int>> seen) {
    // start from the top left corner of the terminal
    std::cout << "\033[0;0H";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (q.front() == std::make_tuple(0, i, j)) {
                // blue dot symbol
                std::cout << "\033[1;34m.\033[0m";
            } else if (seen.find({i, j}) != seen.end()) {
                // red dot symbol
                std::cout << "\033[1;31m.\033[0m";
            } else if (grid[i][j] == "#") {
                // grey full square symbol
                std::cout << "\033[1;30m#\033[0m";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::flush;
}

int main() {
    std::vector<std::pair<int, int>> data = read_file("in.txt");
    int rows = 71;
    int cols = 71;
    int bits_to_simulate = 1024;
    // create a grid of rows x cols
    std::vector<std::vector<std::string>> grid(
        rows, std::vector<std::string>(cols, "."));
    // fill the grid with the data # if in the data . if not
    for (int i = 0; i < bits_to_simulate; i++) {
        grid[data[i].first][data[i].second] = "#";
    }
    // BFS
    std::vector<std::pair<int, int>> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    std::queue<std::tuple<int, int, int>> q; // d, x, y
    q.push({0, 0, 0});
    std::set<std::pair<int, int>> seen;

    while (!q.empty()) {
        int d = std::get<0>(q.front());
        int x = std::get<1>(q.front());
        int y = std::get<2>(q.front());
        q.pop();

        if (x == 70 && y == 70) {
            std::cout << "Found path" << std::endl;
            std::cout << d << std::endl;
            break;
        }

        if (seen.find({x, y}) != seen.end()) {
            continue;
        }
        seen.insert({x, y});

        for (const auto &dir : dirs) {
            int nx = x + dir.first;
            int ny = y + dir.second;
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols &&
                grid[nx][ny] != "#") {
                q.push({d + 1, nx, ny});
            }
        }
        display_bfs(grid, rows, cols, q, seen);
    }
}
