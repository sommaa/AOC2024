#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
#include <string>

struct state {
    int x, y;  // Current position
    int dir;   // Current direction (0: East, 1: North, 2: West, 3: South)
    int score; // Current total score

    // Custom comparator for priority queue (lower score first)
    bool operator > (const state& other) const {
        return score > other.score;
    }
};

class reindeer_maze {
private:
    std::vector<std::string> maze;
    int start_x, start_y, end_x, end_y;
    const int dx[4] = {1, 0, -1, 0};  // East, North, West, South
    const int dy[4] = {0, -1, 0, 1};

public:
    void read_maze(const std::string& filename) {
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

    int solve() {
        // Priority queue to track states, prioritizing lower scores
        std::priority_queue<state, std::vector<state>, std::greater<state>> pq;
        
        // 3D visited array to track visited states (x, y, direction)
        std::vector<std::vector<std::vector<int>>> visited(
            maze[0].size(), 
            std::vector<std::vector<int>>(
                maze.size(), 
                std::vector<int>(4, std::numeric_limits<int>::max())
            )
        );

        // Initial state: start at (start_x, start_y), facing East
        pq.push({start_x, start_y, 0, 0});
        visited[start_x][start_y][0] = 0;

        while (!pq.empty()) {
            state current = pq.top();
            pq.pop();

            // Reached end tile?
            if (current.x == end_x && current.y == end_y) {
                return current.score;
            }

            // Try moving forward
            int nx = current.x + dx[current.dir];
            int ny = current.y + dy[current.dir];
            
            if (is_valid_move(nx, ny) && 
                current.score + 1 < visited[nx][ny][current.dir]) {
                state forward = {nx, ny, current.dir, current.score + 1};
                pq.push(forward);
                visited[nx][ny][current.dir] = forward.score;
            }

            // Try rotating clockwise
            int clock_dir = (current.dir + 1) % 4;
            if (current.score + 1000 < visited[current.x][current.y][clock_dir]) {
                state clockwise = {current.x, current.y, clock_dir, current.score + 1000};
                pq.push(clockwise);
                visited[current.x][current.y][clock_dir] = clockwise.score;
            }

            // Try rotating counterclockwise
            int counter_clock_dir = (current.dir - 1 + 4) % 4;
            if (current.score + 1000 < visited[current.x][current.y][counter_clock_dir]) {
                state counter_clockwise = {current.x, current.y, counter_clock_dir, current.score + 1000};
                pq.push(counter_clockwise);
                visited[current.x][current.y][counter_clock_dir] = counter_clockwise.score;
            }
        }

        return -1;  // No path found
    }

private:
    bool is_valid_move(int x, int y) {
        return x >= 0 && x < maze[0].size() && 
               y >= 0 && y < maze.size() && 
               maze[y][x] != '#';
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <maze_file>" << std::endl;
        return 1;
    }

    reindeer_maze solver;
    solver.read_maze(argv[1]);
    int lowest_score = solver.solve();
    
    std::cout << "Lowest possible score: " << lowest_score << std::endl;
    return 0;
}
