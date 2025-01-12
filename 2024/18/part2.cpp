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

bool simulate_path(const std::vector<std::vector<std::string>> &grid, int rows,
                   int cols) {
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
            return true;
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
    }
    return false;
}

int main() {
    std::vector<std::pair<int, int>> data = read_file("in.txt");
    int rows = 71;
    int cols = 71;
    int bits_to_simulate = data.size();
    // create a grid of rows x cols
    std::vector<std::vector<std::string>> grid(
        rows, std::vector<std::string>(cols, "."));
    // fill the grid with the data # if in the data . if not
    for (int i = 0; i < bits_to_simulate; i++) {
        grid[data[i].first][data[i].second] = "#";
        simulate_path(grid, rows, cols);
        if (!simulate_path(grid, rows, cols)) {
            std::cout << "No path found at: " << data[i].first << ","
                      << data[i].second << std::endl;
            break;
        }
    }
}
