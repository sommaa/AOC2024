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

// find the starting pouint64_t "S"
std::pair<uint64_t, uint64_t> find_start(std::vector<std::string> maze) {
    for (uint64_t i = 0; i < maze.size(); i++) {
        for (uint64_t j = 0; j < maze[i].size(); j++) {
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
    struct hash<std::pair<uint64_t, uint64_t>> {
        size_t operator()(const std::pair<uint64_t, uint64_t> &p) const {
            size_t h1 = std::hash<uint64_t>()(p.first);
            size_t h2 = std::hash<uint64_t>()(p.second);
            // mixing hash functions
            h1 ^= h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
            return h1;
        }
    };
}

uint64_t count_steps(std::vector<std::string> &maze, std::pair<uint64_t, uint64_t> start, uint64_t n_steps) {
    std::deque<std::tuple<uint64_t, uint64_t, uint64_t>> q; // row, col, steps 
    q.push_back({start.first, start.second, 0});
    std::unordered_set<std::pair<uint64_t, uint64_t>, std::hash<std::pair<uint64_t, uint64_t>>> visited;
    std::unordered_set<std::pair<uint64_t, uint64_t>, std::hash<std::pair<uint64_t, uint64_t>>> possible_endings;
    std::vector<std::pair<uint64_t, uint64_t>> adj = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    visited.insert({start.first, start.second});
    q.push_back({start.first, start.second, n_steps});

    while (!q.empty()) {
        uint64_t row = std::get<0>(q.front());
        uint64_t col = std::get<1>(q.front());
        uint64_t steps = std::get<2>(q.front());
        q.pop_front();

        if (steps % 2 == 0) {
            possible_endings.insert({row, col});
        }
        if (steps == 0) {
            continue;
        }

        for (auto &d : adj) {
            uint64_t new_row = row + d.first;
            uint64_t new_col = col + d.second;
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
    std::pair<uint64_t, uint64_t> start = find_start(maze);
    uint64_t steps = 26501365;

    // assert the maze is square 
    assert(maze.size() == maze[0].size());
    // assert the start size is in the center of the maze 
    assert(start.first == maze.size() / 2 && start.second == maze.size() / 2);
    uint64_t grid_size = (uint64_t)maze.size();
    // check dimensions of the hyper-grid
    uint64_t hyper_grid_size = (uint64_t) steps / grid_size - 1;
    // reachable odd grids 
    uint64_t reachable_odd = std::pow(hyper_grid_size / 2 * 2 + 1, 2);
    // reachable even grids 
    uint64_t reachable_even = std::pow((hyper_grid_size + 1) / 2 * 2, 2);
    // fully reachable grids 
    uint64_t odds = count_steps(maze, start, steps * 2 + 1) - 1;
    uint64_t evens = count_steps(maze, start, steps * 2);

    // not fully reachable grids 
    // corners
    uint64_t corner_north = count_steps(maze, std::make_pair(grid_size - 1, start.second), grid_size - 1);
    uint64_t corner_south = count_steps(maze, std::make_pair(0, start.second), grid_size - 1);
    uint64_t corner_east = count_steps(maze, std::make_pair(start.first, grid_size - 1), grid_size - 1);
    uint64_t corner_west = count_steps(maze, std::make_pair(start.first, 0), grid_size - 1);

    // small edges 
    uint64_t sedge_north_east = count_steps(maze, std::make_pair(grid_size - 1, 0), grid_size / 2 - 1);
    uint64_t sedge_north_west = count_steps(maze, std::make_pair(grid_size - 1, grid_size - 1), grid_size / 2 - 1);
    uint64_t sedge_south_east = count_steps(maze, std::make_pair(0, 0), grid_size / 2 - 1);
    uint64_t sedge_south_west = count_steps(maze, std::make_pair(0, grid_size - 1), grid_size / 2 - 1);

    // large edges 
    uint64_t ledge_north_east = count_steps(maze, std::make_pair(grid_size - 1, 0), grid_size * 3 / 2 - 1) - 1;
    uint64_t ledge_north_west = count_steps(maze, std::make_pair(grid_size - 1, grid_size - 1), grid_size * 3 / 2 - 1) - 1;
    uint64_t ledge_south_east = count_steps(maze, std::make_pair(0, 0), grid_size * 3 / 2 - 1) - 1;
    uint64_t ledge_south_west = count_steps(maze, std::make_pair(0, grid_size - 1), grid_size * 3 / 2 - 1) - 1;

    // sum of all reachable grids 
    uint64_t reachable = reachable_odd * odds
                          + reachable_even * evens
                          + corner_north + corner_south + corner_east + corner_west
                          + (sedge_north_east + sedge_north_west + sedge_south_east + sedge_south_west) * (hyper_grid_size + 1)
                          + (ledge_north_east + ledge_north_west + ledge_south_east + ledge_south_west) * (hyper_grid_size);

    std::cout << reachable << std::endl;
    return 0;
}

