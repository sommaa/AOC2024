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

    // get the quadrant of the robot central lines count as no quadrant
    int getQuadrant(int rows, int cols) {
        int x = px;
        int y = py;
        if (x == cols / 2 || y == rows / 2) {
            return 0;
        }
        if (x < cols / 2 && y < rows / 2) {
            return 1;
        }
        if (x > cols / 2 && y < rows / 2) {
            return 2;
        }
        if (x < cols / 2 && y > rows / 2) {
            return 3;
        }
        if (x > cols / 2 && y > rows / 2) {
            return 4;
        }
        return 0;
    }
};

int main() {
    std::vector<Robot> robots;
    std::string line;

    // Read input lines
    while (std::getline(std::cin, line)) {
        if (line.empty())
            break;
        robots.emplace_back(line);
    }

    int time_span = 100;
    int rows = 103;
    int cols = 101;

    // Move robots by time_span steps
    for (int i = 0; i < time_span; i++) {
        for (auto &robot : robots) {
            robot.move(rows, cols);
        }
    }

    // get the quadrant of the robot central lines count as no quadrant
    std::vector<int> quadrants(5, 0);
    for (auto &robot : robots) {
        quadrants[robot.getQuadrant(rows, cols)]++;
    }

    // multiply the number of robots in each quadrant
    int result = quadrants[1] * quadrants[2] * quadrants[3] * quadrants[4];
    std::cout << "Result: " << result << std::endl;

    return 0;
}
