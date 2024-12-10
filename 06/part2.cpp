#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// Transform a rectangle of text from an input file into a 2D matrix of
// characters
std::vector<std::vector<char>> import_input(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file " + filename);
    }

    std::vector<std::vector<char>> input;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) { // Skip empty lines
            input.emplace_back(line.begin(), line.end());
        }
    }
    return input;
}

// function that does the while loop in part1.cpp
// and returns the sum of visited positions
bool stuck_loop(const std::vector<std::vector<char>> &input, int start_i,
                int start_j, std::vector<std::vector<int>> &visited) {
    bool stuck = false;
    int rows = input.size();
    int cols = input[0].size();
    int count_looping = 0;
    // Define directions
    std::vector<std::pair<int, int>> directions = {
        {-1, 0}, // up
        {0, 1},  // right
        {1, 0},  // down
        {0, -1}  // left
    };

    // Mark the starting position as visited
    int i = start_i, j = start_j;
    visited[i][j] = 1;

    int dir_idx = 0; // Index to track the current direction

    while (true) {
        // Calculate the next position
        int nx = i + directions[dir_idx].first;
        int ny = j + directions[dir_idx].second;

        // Check bounds
        if (nx < 0 || nx >= rows || ny < 0 || ny >= cols) {
            break;
        }

        // If the next position is a wall, turn right
        if (input[nx][ny] == '#') {
            dir_idx = (dir_idx + 1) % directions.size();
        } else {
            // Move to the next position
            i = nx;
            j = ny;

            // Mark the position as visited
            visited[i][j] = 1;
        }
        count_looping++;
        if (count_looping >= rows * cols) {
            stuck = true;
            break;
        }
    }

    // Sum the visited matrix
    int sum = 0;
    for (int ii = 0; ii < rows; ii++) {
        for (int jj = 0; jj < cols; jj++) {
            sum += visited[ii][jj];
        }
    }
    return stuck;
}

int main() {
    // Import the input file
    std::vector<std::vector<char>> input = import_input("in.txt");
    int rows = input.size();
    int cols = input[0].size();
    // Create a matrix with the size of the input matrix filled with zeros
    std::vector<std::vector<int>> visited(rows, std::vector<int>(cols, 0));

    // Find ^ in the input
    int start_i = -1, start_j = -1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (input[i][j] == '^') {
                start_i = i;
                start_j = j;
                break;
            }
        }
        if (start_i != -1)
            break; // Exit outer loop if ^ is found
    }

    if (start_i == -1 || start_j == -1) {
        throw std::runtime_error(
            "Error: Starting position ^ not found in input.");
    }

    // brute force # wall new position
    int possible_stuck = 0;
    for (int idx = 0; idx < rows * cols; idx++) {
        int i = idx / cols;
        int j = idx % cols;
    
        if (input[i][j] == '.') {
            std::vector<std::vector<int>> visited(rows, std::vector<int>(cols, 0));
            
            input[i][j] = '#';
    
            if (stuck_loop(input, start_i, start_j, visited)) {
                possible_stuck++;
            }
    
            input[i][j] = '.'; // Revert the wall
        }
    }
    std::cout << "Total possible walls: " << possible_stuck << std::endl;
    return 0;
}
