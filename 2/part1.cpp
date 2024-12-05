#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Import the input file as a vector of strings
std::vector<std::string> import_input(const std::string &filename) {
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

// Split the string into a vector of integers
std::vector<int> split_string_to_numbers(const std::string &str) {
    std::istringstream stream(str);
    std::vector<int> numbers;
    int num;

    while (stream >> num) {
        numbers.push_back(num);
    }

    return numbers;
}

int main() {
    try {
        // Import the input file
        std::vector<std::string> input = import_input("in.txt");
        if (input.empty()) {
            std::cerr << "Error: Input file is empty." << std::endl;
            return 1;
        }

        int safe = 0;
        for (const std::string &line : input) {
            bool valid = true;

            // Split the string into a vector of numbers
            std::vector<int> numbers = split_string_to_numbers(line);

            // Skip if the line is empty
            if (numbers.empty())
                continue;

            // Check if the sequence is sorted or reverse sorted
            std::vector<int> sorted = numbers;
            std::sort(sorted.begin(), sorted.end());
            std::vector<int> reverse_sorted = sorted;
            std::reverse(reverse_sorted.begin(), reverse_sorted.end());
            if (numbers != sorted && numbers != reverse_sorted) {
                valid = false;
                continue;
            }

            // Check absolute differences between consecutive numbers
            for (size_t j = 0; j < numbers.size() - 1; j++) {
                int diff = std::abs(numbers[j] - numbers[j + 1]);
                if (diff > 3 || diff < 1) {
                    valid = false;
                    break;
                }
            }

            // If the sequence is valid, increment the safe counter
            if (valid) {
                safe++;
            }
        }
        std::cout << "Safe sequences count: " << safe << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
