#include <bits/stdc++.h>

// import maze 
std::vector<std::string> import_maze() {
    std::vector<std::string> maze;
    std::string line;
    while (std::getline(std::cin, line)) {
        maze.push_back(line);
    }
    return maze;
}

// find the starting point "S"
std::pair<int, int> find_start(std::vector<std::string> maze) {
    for (int i = 0; i < maze.size(); i++) {
        for (int j = 0; j < maze[i].size(); j++) {
            if (maze[i][j] == 'S') {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

// pair hash function 
namespace std {
    template <>
    struct hash<std::pair<int, int>> {
        size_t operator()(const std::pair<int, int> &p) const {
            return std::hash<int>()(p.first) ^ std::hash<int>()(p.second);
        }
    };
}

int count_steps(std::vector<std::string> &maze, std::pair<int, int> start, int n_steps) {
    std::deque<std::tuple<int, int, int>> q; // row, col, steps 
    q.push_back({start.first, start.second, 0});
    std::unordered_set<std::pair<int, int>, std::hash<std::pair<int, int>>> visited;
    std::unordered_set<std::pair<int, int>, std::hash<std::pair<int, int>>> possible_endings;
    std::vector<std::pair<int, int>> adj = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    visited.insert({start.first, start.second});
    q.push_back({start.first, start.second, n_steps});

    while (!q.empty()) {
        int row = std::get<0>(q.front());
        int col = std::get<1>(q.front());
        int steps = std::get<2>(q.front());
        q.pop_front();

        if (steps % 2 == 0) {
            possible_endings.insert({row, col});
        }
        if (steps == 0) {
            continue;
        }

        for (auto &d : adj) {
            int new_row = row + d.first;
            int new_col = col + d.second;
            if (new_row < 0 || new_row >= maze.size() || new_col < 0 || new_col >= maze[0].size()) {
                continue;
            }
            if (maze[new_row][new_col] == '#') {
                continue;
            }
            if (visited.find({new_row, new_col}) != visited.end()) {
                continue;
            }
            visited.insert({new_row, new_col});
            q.push_back({new_row, new_col, steps - 1});
        }
    }
    return possible_endings.size();
}

int main() {
    std::vector<std::string> maze = import_maze();
    std::pair<int, int> start = find_start(maze);
    std::cout << count_steps(maze, start, 64) << std::endl;
    return 0;
}

