#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype> // for isdigit
#include <stdexcept>

// Import the input file as a vector of strings
std::vector<std::string> import_input(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file " + filename);
    }

    std::vector<std::string> input;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) { // Skip empty lines
            input.push_back(line);
        }
    }
    return input;
}

int main() {
    // Import the input file
    std::vector<std::string> input = import_input("in.txt");

    int total_sum = 0;
    // Loop through lines
    for (int i = 0; i < input.size(); i++) {
        // Get the position of "mul(" in the line; it can appear multiple times
        std::vector<int> positions;
        std::string line = input[i];
        std::string search = "mul(";

        size_t start = 0;
        while (true) {
            size_t pos = line.find(search, start);
            if (pos == std::string::npos) {
                break; // Exit if no more matches are found
            }
            positions.push_back(pos); // Store the position
            start = pos + search.size(); // Move to the next search position
        }

        // Print positions for debugging
        std::cout << "Line " << i + 1 << " Positions: ";
        for (int pos : positions) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;

        // Extract numbers and calculate the sum
        int sum = 0;
        for (int pos : positions) {
            std::string num1, num2;
            size_t k = pos + search.size(); // Start after "mul("

            // Extract num1
            while (k < line.size() && line[k] != ',' && line[k] != ')') {
                if (isdigit(line[k])) {
                    num1 += line[k];
                } else {
                    num1 = ""; // Reset if invalid character is found
                    break;
                }
                k++;
            }
            k++; // Skip the ','

            // Extract num2
            while (k < line.size() && line[k] != ')') {
                if (isdigit(line[k])) {
                    num2 += line[k];
                } else {
                    num2 = ""; // Reset if invalid character is found
                    break;
                }
                k++;
            }

            // Ensure valid numbers
            if (num1.empty() || num2.empty()) {
                std::cerr << "Error: Invalid number format at line " << i + 1 << std::endl;
                continue;
            }

            // Convert to integers and calculate product
            int n1 = std::stoi(num1);
            int n2 = std::stoi(num2);
            sum += n1 * n2;

            // Print extracted numbers
            std::cout << "Num1: " << n1 << ", Num2: " << n2 << ", Product: " << n1 * n2 << std::endl;
        }

        // Print total sum for the line
        std::cout << "Total Sum for Line " << i + 1 << ": " << sum << std::endl;
        total_sum += sum;
    }
    std::cout << "Total Sum: " << total_sum << std::endl;

    return 0;
}

