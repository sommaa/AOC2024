#include <iostream>
#include <string>
#include <utility>
#include <vector>

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

int find_max(const std::vector<std::vector<int>> &distance) {
    int max_distance = 0;
    for (const auto &row : distance)
        for (int d : row)
            if (d != -1) max_distance = std::max(max_distance, d);
    return max_distance;
}

int main() {
    auto maze = import_maze();
    auto start = find_start(maze);
    auto distance = bfs(maze, start.first, start.second);
    std::cout << find_max(distance) << std::endl;
    return 0;
}
