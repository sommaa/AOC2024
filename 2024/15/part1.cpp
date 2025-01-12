#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

class robot_puzzle {
  private:
    std::vector<std::string> grid;
    std::pair<int, int> robot_pos;
    std::string moves;
    bool display_game = true;

    void find_initial_positions() {
        robot_pos = {-1, -1};
        for (int y = 0; y < grid.size(); ++y) {
            for (int x = 0; x < grid[y].length(); ++x) {
                if (grid[y][x] == '@') {
                    robot_pos = {y, x};
                    grid[y][x] = '.'; // Replace robot with empty space
                }
            }
        }
    }

  public:
    void readFile(const std::string &filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        // Read grid
        std::string line;
        while (std::getline(file, line) && line[0] == '#') {
            grid.push_back(line);
        }

        // Read moves
        while (std::getline(file, line)) {
            moves += line;
        }
        find_initial_positions();
    }

    void simulate_moves() {
        std::unordered_map<char, std::pair<int, int>> directions = {
            {'<', {0, -1}}, {'>', {0, 1}}, {'^', {-1, 0}}, {'v', {1, 0}}};

        for (char move : moves) {
            auto dir = directions[move];
            int new_y = robot_pos.first + dir.first;
            int new_x = robot_pos.second + dir.second;

            if (grid[new_y][new_x] == '#') {
                std::cout << "Move: " << move << std::endl;
                display_state();
                continue; // Cannot move into wall
            }

            if (grid[new_y][new_x] == '.') {
                robot_pos = {new_y, new_x}; // Move robot to empty space
                std::cout << "Move: " << move << std::endl;
                display_state();
                continue;
            }

            if (grid[new_y][new_x] == 'O') {
                // see if after the box there is a space or a box or a wall
                int new_box_y = new_y + dir.first;
                int new_box_x = new_x + dir.second;

                // while the next position is a box
                while (grid[new_box_y][new_box_x] == 'O') {
                    new_box_y += dir.first;
                    new_box_x += dir.second;
                }
                if (grid[new_box_y][new_box_x] == '.') {
                    // move robot to new_x, new_y and move box to new_box_x,
                    // new_box_y put an empty space in the previous box position
                    robot_pos = {new_y, new_x};
                    grid[new_y][new_x] = '.';
                    grid[new_box_y][new_box_x] = 'O';
                }
                if (grid[new_box_y][new_box_x] == '#') {
                    std::cout << "Move: " << move << std::endl;
                    display_state();
                    continue;
                }
            }
            if (display_game) {
                display_state(); // Display state after each move
            }
        }
    }

    void display_state() {
        int col = grid[0].length();
        int row = grid.size();

        // Optional: Clear the screen before starting (to make sure the output
        // starts fresh).
        std::cout << "\033[2J\033[H"; // ANSI escape to clear the screen

        // Loop through each row
        for (int i = 0; i < row; i++) {
            // Clear the line before printing the next row and move the cursor
            // to the beginning of the line
            std::cout
                << "\033[2K\r"; // Clear the line and move cursor to the start

            // Loop through each column in the current row
            for (int j = 0; j < col; j++) {
                if (robot_pos.first == i && robot_pos.second == j) {
                    // circle green symbol for the robot
                    std::cout << "\033[32m\u25CF\033[0m";
                } else if (grid[i][j] == 'O') {
                    // square blue symbol for the box
                    std::cout << "\033[31m\u2588\033[0m";
                } else if (grid[i][j] == '#') {
                    // wall in dark gray as a full block
                    std::cout << "\033[90m\u2588\033[0m";
                } else {
                    // empty space
                    std::cout << " ";
                }
            }

            // After printing a row, move the cursor to the next line
            std::cout << std::flush;

            // Print a newline after each row (except for the last one, if you
            // don't want extra lines)
            if (i < row - 1) {
                std::cout << "\n"; // Add newline at the end of the row, but not
                                   // after the last row
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    int get_sum_gps_coordinates() {
        // find all the boxes positions
        std::vector<std::pair<int, int>> boxes;
        for (int y = 0; y < grid.size(); ++y) {
            for (int x = 0; x < grid[y].length(); ++x) {
                if (grid[y][x] == 'O') {
                    boxes.push_back({y, x});
                }
            }
        }

        // sum all the boxes positions with 100*y + x rule
        int sum = 0;
        for (auto box : boxes) {
            sum += 100 * box.first + box.second;
        }
        return sum;
    }
};

int main() {
    robot_puzzle puzzle;
    puzzle.readFile("in.txt");
    puzzle.display_state();
    puzzle.simulate_moves();
    std::cout << "Sum of GPS coordinates: " << puzzle.get_sum_gps_coordinates()
              << std::endl;
    return 0;
}
