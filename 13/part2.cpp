#include <Eigen/Dense>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct ButtonCoordinates {
    long long buttonAX, buttonAY;
    long long buttonBX, buttonBY;
    long long prizeX, prizeY;
};

ButtonCoordinates parseInputGroup() {
    ButtonCoordinates coord;
    std::string line;

    // Read Button A coordinates
    std::getline(std::cin, line);
    sscanf(line.c_str(), "Button A: X+%lld, Y+%lld", &coord.buttonAX,
           &coord.buttonAY);

    // Read Button B coordinates
    std::getline(std::cin, line);
    sscanf(line.c_str(), "Button B: X+%lld, Y+%lld", &coord.buttonBX,
           &coord.buttonBY);

    // Read Prize coordinates + and add 10000000000000 to its value
    std::getline(std::cin, line);
    sscanf(line.c_str(), "Prize: X=%lld, Y=%lld", &coord.prizeX, &coord.prizeY);
    coord.prizeX += 10000000000000;
    coord.prizeY += 10000000000000;

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

    long long A_cost = 3;
    long long B_cost = 1;
    long long cost = 0;

    for (size_t i = 0; i < results.size(); ++i) {
        const auto &coord = results[i];
        std::cout << "Group " << (i + 1) << " Results:\n";
        std::cout << "Button A: X+" << coord.buttonAX << ", Y+"
                  << coord.buttonAY << "\n";
        std::cout << "Button B: X+" << coord.buttonBX << ", Y+"
                  << coord.buttonBY << "\n";
        std::cout << "Prize: X=" << coord.prizeX << ", Y=" << coord.prizeY
                  << "\n";

        // Use dynamic-sized matrices for larger numbers
        Eigen::MatrixXd A(2, 2);
        Eigen::VectorXd b(2);

        // Fill the matrix and vector
        A << static_cast<double>(coord.buttonAX),
            static_cast<double>(coord.buttonBX),
            static_cast<double>(coord.buttonAY),
            static_cast<double>(coord.buttonBY);
        b << static_cast<double>(coord.prizeX),
            static_cast<double>(coord.prizeY);

        // Solve the system using QR decomposition for better numerical
        // stability
        Eigen::VectorXd press = A.colPivHouseholderQr().solve(b);

        // Round the values of x to the nearest long longeger
        long long pressA = std::round(press(0));
        long long pressB = std::round(press(1));

        // Check if the solution is valid with tolerance for floating-polong
        // long errors
        bool isValidSolution =
            std::abs(coord.buttonAX * pressA + coord.buttonBX * pressB -
                     coord.prizeX) < 1e-7 &&
            std::abs(coord.buttonAY * pressA + coord.buttonBY * pressB -
                     coord.prizeY) < 1e-7;

        if (isValidSolution) {
            std::cout << "Press A: " << pressA << ", Press B: " << pressB
                      << "\n";
            cost += pressA * A_cost + pressB * B_cost;
        } else {
            std::cout << "No solution found\n";
        }
    }
    std::cout << "Total cost: " << cost << std::endl;

    return 0;
}
