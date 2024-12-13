#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

struct Region {
    int area;
    std::vector<std::pair<int, int>> horizontal_sides;
    std::vector<std::pair<int, int>> vertical_sides;
};

Region fill_region(int sx, int sy, const std::vector<int> &grid, int width,
                   int height, std::vector<bool> &seen) {
    int area = 0;
    std::vector<std::pair<int, int>> horizontal_sides;
    std::vector<std::pair<int, int>> vertical_sides;
    std::queue<std::pair<int, int>> queue;

    queue.push({sx, sy});
    seen[sy * width + sx] = true;

    while (!queue.empty()) {
        auto [x, y] = queue.front();
        queue.pop();

        int c = grid[y * width + x];
        area++;

        std::vector<std::pair<int, int>> directions = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        for (const auto &[dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;
            int ni = ny * width + nx;

            if (nx >= 0 && ny >= 0 && nx < width && ny < height &&
                grid[ni] == c) {
                if (!seen[ni]) {
                    seen[ni] = true;
                    queue.push({nx, ny});
                }
            } else if (dx == 1 || dx == -1) {
                vertical_sides.push_back({y, x * 4 + dx});
            } else {
                horizontal_sides.push_back({x, y * 4 + dy});
            }
        }
    }

    return {area, horizontal_sides, vertical_sides};
}

void remove_connected(std::pair<int, int> s,
                      std::vector<std::pair<int, int>> &sides) {
    int a = s.first + 1;
    while (true) {
        auto it = std::find_if(sides.begin(), sides.end(),
                               [a, &s](const std::pair<int, int> &p) {
                                   return p.first == a && p.second == s.second;
                               });

        if (it == sides.end())
            break;

        *it = sides.back();
        sides.pop_back();
        a++;
    }

    a = s.first - 1;
    while (true) {
        auto it = std::find_if(sides.begin(), sides.end(),
                               [a, &s](const std::pair<int, int> &p) {
                                   return p.first == a && p.second == s.second;
                               });

        if (it == sides.end())
            break;

        *it = sides.back();
        sides.pop_back();
        a--;
    }
}

int main() {
    std::ifstream input_file("in.txt");
    if (!input_file) {
        std::cerr << "Could not read file" << std::endl;
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input_file, line)) {
        lines.push_back(line);
    }

    int width = lines[0].length();
    int height = lines.size();

    std::vector<int> grid;
    for (const auto &l : lines) {
        for (char c : l) {
            grid.push_back(static_cast<int>(c));
        }
    }

    std::vector<bool> seen(grid.size(), false);

    int result = 0;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (seen[y * width + x]) {
                continue;
            }

            // Fill region
            Region region = fill_region(x, y, grid, width, height, seen);

            // Find connected side tiles and count sides
            int n_sides = 0;
            std::vector<std::vector<std::pair<int, int>>> side_collections = {
                region.horizontal_sides, region.vertical_sides};

            for (auto &sides : side_collections) {
                while (!sides.empty()) {
                    auto s = sides.back();
                    sides.pop_back();
                    remove_connected(s, sides);
                    n_sides++;
                }
            }

            result += region.area * n_sides;
        }
    }

    std::cout << result << std::endl;

    return 0;
}
