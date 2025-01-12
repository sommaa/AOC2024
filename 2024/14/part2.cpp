#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Struct to represent a robot's position and velocity
struct Robot {
    int px; // x position
    int py; // y position
    int vx; // x velocity
    int vy; // y velocity

    // Constructor to initialize robot from a string input
    Robot(const std::string &input) {
        // Remove 'p=' and 'v=' from the input
        std::string cleanedInput = input;
        size_t pPos = cleanedInput.find("p=");
        size_t vPos = cleanedInput.find("v=");

        // Extract position coordinates
        std::string posStr = cleanedInput.substr(pPos + 2, vPos - pPos - 3);
        std::istringstream posStream(posStr);
        char comma;
        posStream >> px >> comma >> py;

        // Extract velocity coordinates
        std::string velStr = cleanedInput.substr(vPos + 2);
        std::istringstream velStream(velStr);
        velStream >> vx >> comma >> vy;
    }

    // Method to move the robot by one time step
    void move(int rows, int cols) {
        px += vx;
        py += vy;
        // Wrap around the grid
        px = (px + cols) % cols;
        py = (py + rows) % rows;
    }
};

// print position of the robots on the grid, empty space is represented by '.'
// and the robot positions is presented as the sun of the robots in that tile
void printGrid(int rows, int cols, std::vector<Robot> &robots) {
    std::vector<std::vector<int>> grid(rows, std::vector<int>(cols, 0));
    for (auto &robot : robots) {
        grid[robot.py][robot.px]++;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == 0) {
                std::cout << ".";
            } else {
                // square symbol
                std::cout << "\u25A0";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

// check if there is any overlap between the robots
bool checkOverlap(int rows, int cols, std::vector<Robot> &robots) {
    std::vector<std::vector<int>> grid(rows, std::vector<int>(cols, 0));
    for (auto &robot : robots) {
        grid[robot.py][robot.px]++;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] > 1) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    std::vector<Robot> robots;
    std::string line;

    // Read input lines
    while (std::getline(std::cin, line)) {
        if (line.empty())
            break;
        robots.emplace_back(line);
    }

    int time_span = 10000000;
    int rows = 103;
    int cols = 101;

    // Move robots by time_span steps
    for (int i = 0; i < time_span; i++) {
        for (auto &robot : robots) {
            robot.move(rows, cols);
        }
        if (!checkOverlap(rows, cols, robots)) {
            printGrid(rows, cols, robots);
            std::cout << "Time: " << i + 1 << std::endl;
            break;
        }
    }
    return 0;
}
