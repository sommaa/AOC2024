#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct ButtonCoordinates {
    int buttonAX, buttonAY;
    int buttonBX, buttonBY;
    int prizeX, prizeY;
};

ButtonCoordinates parseInputGroup() {
    ButtonCoordinates coord;
    std::string line;

    // Read Button A coordinates
    std::getline(std::cin, line);
    sscanf(line.c_str(), "Button A: X+%d, Y+%d", &coord.buttonAX,
           &coord.buttonAY);

    // Read Button B coordinates
    std::getline(std::cin, line);
    sscanf(line.c_str(), "Button B: X+%d, Y+%d", &coord.buttonBX,
           &coord.buttonBY);

    // Read Prize coordinates
    std::getline(std::cin, line);
    sscanf(line.c_str(), "Prize: X=%d, Y=%d", &coord.prizeX, &coord.prizeY);

    // skip empty line
    std::getline(std::cin, line);

    return coord;
}

int main() {
    std::vector<ButtonCoordinates> results;

    // Read input until EOF or error
    while (std::cin.peek() != EOF && std::cin.good()) {
        results.push_back(parseInputGroup());
    }

    int A_lim = 100;
    int B_lim = 100;
    int A_cost = 3;
    int B_cost = 1;
    int cost = 0;

    for (size_t i = 0; i < results.size(); ++i) {
        const auto &coord = results[i];
        std::cout << "Group " << (i + 1) << " Results:\n";
        std::cout << "Button A: X+" << coord.buttonAX << ", Y+"
                  << coord.buttonAY << "\n";
        std::cout << "Button B: X+" << coord.buttonBX << ", Y+"
                  << coord.buttonBY << "\n";
        std::cout << "Prize: X=" << coord.prizeX << ", Y=" << coord.prizeY
                  << "\n";

        // brute force
        for (int A = 0; A < A_lim; ++A) {
            for (int B = 0; B < B_lim; ++B) {
                if (coord.buttonAX * A + coord.buttonBX * B == coord.prizeX &&
                    coord.buttonAY * A + coord.buttonBY * B == coord.prizeY) {
                    cost += A * A_cost + B * B_cost;
                    break;
                }
            }
        }
    }
    std::cout << "Total cost: " << cost << std::endl;

    return 0;
}
