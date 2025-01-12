#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

// import the maze from file
std::vector<std::vector<char>> import_maze(std::string filename) {
    std::ifstream file(filename);
    std::vector<std::vector<char>> maze;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        maze.push_back(row);
    }
    return maze;
}

std::vector<int> find_start_end(std::vector<std::vector<char>> maze) {
    std::vector<int> start_end;
    for (int i = 0; i < maze.size(); i++) {
        for (int j = 0; j < maze[0].size(); j++) {
            if (maze[i][j] == 'S') {
                start_end.push_back(i);
                start_end.push_back(j);
            } else if (maze[i][j] == 'E') {
                start_end.push_back(i);
                start_end.push_back(j);
            }
        }
    }
    return start_end;
}

int bfs(const std::vector<std::vector<char>> &grid, std::vector<int> start_end,
        int rows, int cols) {
    std::vector<std::pair<int, int>> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    std::queue<std::tuple<int, int, int>> q; // d, x, y
    q.push({0, start_end[0], start_end[1]});
    int final_x = start_end[2];
    int final_y = start_end[3];
    std::set<std::pair<int, int>> seen;

    while (!q.empty()) {
        int d = std::get<0>(q.front());
        int x = std::get<1>(q.front());
        int y = std::get<2>(q.front());
        q.pop();

        if (x == final_x && y == final_y) {
            return d;
        }

        if (seen.find({x, y}) != seen.end()) {
            continue;
        }
        seen.insert({x, y});

        for (const auto &dir : dirs) {
            int nx = x + dir.first;
            int ny = y + dir.second;
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols &&
                grid[nx][ny] != '#') {
                q.push({d + 1, nx, ny});
            }
        }
    }
    return -1;
}

int main() {
    std::vector<std::vector<char>> maze;
    maze = import_maze("in.txt");
    int rows = maze.size();
    int cols = maze[0].size();
    std::vector<int> start_end = find_start_end(maze);

    int steps = bfs(maze, start_end, rows, cols);
    int reference = steps - 100;
    
    // remove a wall and see if the path is shorter by 100 steps 
    int result = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == '#') {
                maze[i][j] = '.';
                int new_steps = bfs(maze, start_end, rows, cols);
                if (new_steps <= reference) {
                    result++;
                }
                maze[i][j] = '#';
            }
        }
    }
    std::cout << result << std::endl;
    return 0;
}
