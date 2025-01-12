#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <algorithm>
#include <cassert>

std::vector<std::string> import_maze() {
    std::vector<std::string> maze; std::string line;
    while (std::getline(std::cin, line)) maze.push_back(line);
    return maze;
}

std::pair<int, int> d_up{-1, 0}, d_down{1, 0}, d_left{0, -1}, d_right{0, 1};

bool is_valid_next(const std::vector<std::string> &maze, int x, int y, std::pair<int, int> dir) {
    char c = maze[x + dir.first][y + dir.second];
    return (c == '|' && (dir == d_up || dir == d_down)) ||
           (c == '-' && (dir == d_left || dir == d_right)) ||
           (c == '7' && (dir == d_up || dir == d_right)) ||
           (c == 'L' && (dir == d_down || dir == d_left)) ||
           (c == 'J' && (dir == d_down || dir == d_right)) ||
           (c == 'F' && (dir == d_up || dir == d_left));
}

bool is_valid_local(const std::vector<std::string> &maze, int x, int y, std::pair<int, int> dir) {
    char c = maze[x][y];
    return c == 'S' ||
           (c == '|' && (dir == d_up || dir == d_down)) ||
           (c == '-' && (dir == d_left || dir == d_right)) ||
           (c == '7' && (dir == d_left || dir == d_down)) ||
           (c == 'L' && (dir == d_up || dir == d_right)) ||
           (c == 'J' && (dir == d_up || dir == d_left)) ||
           (c == 'F' && (dir == d_down || dir == d_right));
}

std::vector<std::vector<int>> bfs(const std::vector<std::string> &maze, int start_x, int start_y) {
    std::vector<std::vector<int>> distance(maze.size(), std::vector<int>(maze[0].size(), -1));
    std::vector<std::pair<int, int>> queue{{start_x, start_y}};
    distance[start_x][start_y] = 0;
    
    while (!queue.empty()) {
        auto [x, y] = queue.front(); queue.erase(queue.begin());
        for (auto dir : {d_up, d_down, d_left, d_right}) {
            int nx = x + dir.first, ny = y + dir.second;
            if (nx >= 0 && nx < maze.size() && ny >= 0 && ny < maze[0].size() && 
                distance[nx][ny] == -1 && is_valid_next(maze, x, y, dir) && 
                is_valid_local(maze, x, y, dir)) {
                distance[nx][ny] = distance[x][y] + 1;
                queue.push_back({nx, ny});
            }
        }
    }
    return distance;
}

std::pair<int, int> find_start(const std::vector<std::string> &maze) {
    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++)
            if (maze[i][j] == 'S') return {i, j};
    return {-1, -1};
}

void remove_pipes(std::vector<std::string> &maze, std::vector<std::vector<int>> &distance) {
    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++)
            if (distance[i][j] == -1 && maze[i][j] != '.') maze[i][j] = '.';
}

std::set<std::pair<int, int>> find_outside(const std::vector<std::string> &maze) {
    std::set<std::pair<int, int>> outside;
    for (int i = 0; i < maze.size(); i++) {
        bool inside = false;
        int upwards = -1;
        for (int j = 0; j < maze[i].size(); j++) {
            if (maze[i][j] == '|') { assert(upwards == -1); inside = !inside; }
            else if (maze[i][j] == '-') assert(upwards != -1);
            else if (maze[i][j] == 'L' || maze[i][j] == 'F') {
                assert(upwards == -1);
                upwards = maze[i][j] == 'L';
            }
            else if (maze[i][j] == 'J' || maze[i][j] == '7') {
                assert(upwards != -1);
                if (maze[i][j] != (upwards ? 'J' : '7')) inside = !inside;
                upwards = -1;
            }
            if (!inside) outside.insert({i, j});
        }
    }
    return outside;
}

void replace_start(std::vector<std::string> &maze, std::vector<std::vector<int>> &distance, std::pair<int, int> start) {
    std::vector<std::pair<int, int>> dirs;
    for (auto dir : {d_up, d_down, d_left, d_right}) {
        int nx = start.first + dir.first, ny = start.second + dir.second;
        if (nx >= 0 && nx < maze.size() && ny >= 0 && ny < maze[0].size() && distance[nx][ny] != -1)
            dirs.push_back(dir);
    }
    
    if (dirs[0] == d_up && dirs[1] == d_down) maze[start.first][start.second] = '|';
    else if (dirs[0] == d_left && dirs[1] == d_right) maze[start.first][start.second] = '-';
    else if (dirs[0] == d_down && dirs[1] == d_left) maze[start.first][start.second] = '7';
    else if (dirs[0] == d_up && dirs[1] == d_right) maze[start.first][start.second] = 'L';
    else if (dirs[0] == d_up && dirs[1] == d_left) maze[start.first][start.second] = 'J';
    else if (dirs[0] == d_down && dirs[1] == d_right) maze[start.first][start.second] = 'F';
}

int count_inside(const std::vector<std::string> &maze, const std::set<std::pair<int, int>> &outside) {
    int inside = 0;
    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++)
            if (maze[i][j] == '.' && outside.find({i, j}) == outside.end()) inside++;
    return inside;
}

int main() {
    auto maze = import_maze();
    auto start = find_start(maze);
    auto distance = bfs(maze, start.first, start.second);
    remove_pipes(maze, distance);
    replace_start(maze, distance, start);
    auto outside = find_outside(maze);
    std::cout << count_inside(maze, outside) << std::endl;
    return 0;
}
