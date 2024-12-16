#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <vector>

struct state {
    int x, y;  // Current position
    int dir;   // Current direction (0: East, 1: North, 2: West, 3: South)
    int score; // Current total score
    std::vector<std::pair<int, int>> path; // Path taken so far

    // Custom comparator for priority queue (lower score first)
    bool operator>(const state &other) const { return score > other.score; }
};

class reindeer_maze {
  private:
    std::vector<std::string> maze;
    int start_x, start_y, end_x, end_y;
    const int dx[4] = {1, 0, -1, 0}; // East, North, West, South
    const int dy[4] = {0, -1, 0, 1};
    std::vector<std::pair<int, std::vector<std::pair<int, int>>>> valid_paths;
    int lowest_score = std::numeric_limits<int>::max();

  public:
    void read_maze(const std::string &filename) {
        std::ifstream file(filename);
        std::string line;

        if (!file.is_open()) {
            std::cerr << "Could not open file: " << filename << std::endl;
            exit(1);
        }

        while (std::getline(file, line)) {
            maze.push_back(line);
        }

        // Find start and end positions
        for (int y = 0; y < maze.size(); y++) {
            for (int x = 0; x < maze[y].length(); x++) {
                if (maze[y][x] == 'S') {
                    start_x = x;
                    start_y = y;
                }
                if (maze[y][x] == 'E') {
                    end_x = x;
                    end_y = y;
                }
            }
        }
    }

    std::vector<std::pair<int, std::vector<std::pair<int, int>>>> solve() {
        // Clear previous results
        valid_paths.clear();
        lowest_score = std::numeric_limits<int>::max();

        // Priority queue to track states, prioritizing lower scores
        std::priority_queue<state, std::vector<state>, std::greater<state>> pq;

        // 3D visited array to track visited states (x, y, direction)
        std::vector<std::vector<std::vector<int>>> visited(
            maze[0].size(),
            std::vector<std::vector<int>>(
                maze.size(),
                std::vector<int>(4, std::numeric_limits<int>::max())));

        // Initial state: start at (start_x, start_y), facing East and West
        for (int initial_dir : {0, 2}) {
            state initial_state = {start_x, start_y, initial_dir, 0};
            initial_state.path.push_back({start_x, start_y});
            pq.push(initial_state);
            visited[start_x][start_y][initial_dir] = 0;
        }

        while (!pq.empty()) {
            state current = pq.top();
            pq.pop();

            // If we've found paths with lower scores, skip states with higher
            // scores
            if (current.score > lowest_score)
                break;

            // Reached end tile?
            if (current.x == end_x && current.y == end_y) {
                // If this is a new lowest score, clear previous paths
                if (current.score < lowest_score) {
                    valid_paths.clear();
                    lowest_score = current.score;
                }
                valid_paths.push_back({current.score, current.path});
            }

            // Try moving forward
            int nx = current.x + dx[current.dir];
            int ny = current.y + dy[current.dir];

            if (is_valid_move(nx, ny) && current.score + 1 <= lowest_score &&
                current.score + 1 <= visited[nx][ny][current.dir]) {
                state forward = {nx, ny, current.dir, current.score + 1,
                                 current.path};
                forward.path.push_back({nx, ny});
                pq.push(forward);
                visited[nx][ny][current.dir] = forward.score;
            }

            // Try rotating clockwise
            int clock_dir = (current.dir + 1) % 4;
            if (current.score + 1000 <= lowest_score &&
                current.score + 1000 <=
                    visited[current.x][current.y][clock_dir]) {
                state clockwise = {current.x, current.y, clock_dir,
                                   current.score + 1000, current.path};
                pq.push(clockwise);
                visited[current.x][current.y][clock_dir] = clockwise.score;
            }

            // Try rotating counterclockwise
            int counter_clock_dir = (current.dir - 1 + 4) % 4;
            if (current.score + 1000 <= lowest_score &&
                current.score + 1000 <=
                    visited[current.x][current.y][counter_clock_dir]) {
                state counter_clockwise = {current.x, current.y,
                                           counter_clock_dir,
                                           current.score + 1000, current.path};
                pq.push(counter_clockwise);
                visited[current.x][current.y][counter_clock_dir] =
                    counter_clockwise.score;
            }
        }

        return valid_paths;
    }

    void print_paths() {
        for (const auto &path_info : valid_paths) {
            std::cout << "Path Score: " << path_info.first << std::endl;
            std::cout << "Path: ";
            for (const auto &point : path_info.second) {
                std::cout << "(" << point.first << "," << point.second << ") ";
            }
            std::cout << std::endl << std::endl;
        }
    }

    std::vector<std::string> get_maze() { return maze; }
    int get_lowest_score() { return lowest_score; }

  private:
    bool is_valid_move(int x, int y) {
        return x >= 0 && x < maze[0].size() && y >= 0 && y < maze.size() &&
               maze[y][x] != '#';
    }
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <maze_file>" << std::endl;
        return 1;
    }

    reindeer_maze solver;
    solver.read_maze(argv[1]);
    auto paths = solver.solve();

    std::cout << "Found " << paths.size() << " paths with lowest score of "
              << (paths.empty() ? 0 : paths[0].first) << ":" << std::endl;
    solver.print_paths();

    // Unique points in all paths
    std::vector<std::pair<int, int>> unique_points;
    for (const auto &path_info : paths) {
        for (const auto &point : path_info.second) {
            if (std::find(unique_points.begin(), unique_points.end(), point) ==
                unique_points.end()) {
                unique_points.push_back(point);
            }
        }
    }

    // Print the unique points on the maze
    std::vector<std::string> maze = solver.get_maze();
    for (int y = 0; y < maze.size(); y++) {
        for (int x = 0; x < maze[y].length(); x++) {
            if (std::find(unique_points.begin(), unique_points.end(),
                          std::make_pair(x, y)) != unique_points.end()) {
                std::cout << "O";
            } else {
                std::cout << maze[y][x];
            }
        }
        std::cout << std::endl;
    }
    // Size of unique points
    std::cout << "Unique points: " << unique_points.size() << std::endl;
    return 0;
}
