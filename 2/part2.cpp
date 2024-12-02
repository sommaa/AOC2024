#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <cmath>

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

// Split the string into a vector of integers
std::vector<int> split_string_to_numbers(const std::string& str) {
    std::istringstream stream(str);
    std::vector<int> numbers;
    int num;

    while (stream >> num) {
        numbers.push_back(num);
    }

    return numbers;
}

// Check if a sequence is valid
bool is_valid_sequence(const std::vector<int>& numbers) {
    // Check if the sequence is sorted or reverse sorted
    std::vector<int> sorted = numbers;
    std::sort(sorted.begin(), sorted.end());
    std::vector<int> reverse_sorted = sorted;
    std::reverse(reverse_sorted.begin(), reverse_sorted.end());
    if (numbers != sorted && numbers != reverse_sorted) {
        return false;
    }

    // Check absolute differences between consecutive numbers
    for (size_t j = 0; j < numbers.size() - 1; j++) {
        int diff = std::abs(numbers[j] - numbers[j + 1]);
        if (diff > 3 || diff < 1) {
            return false;
        }
    }

    return true;
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
        int count = 0;
        for (const std::string& line : input) {
            count++;
            // Split the string into numbers
            std::vector<int> numbers = split_string_to_numbers(line);

            bool valid = is_valid_sequence(numbers);
            int dumperCounter = 0;
            if (is_valid_sequence(numbers)) {
                safe++;
                std::cout << count << std::endl;
            } else{
                // Check if removing one number at a time still keeps the sequence valid
                for (size_t i = 0; i < numbers.size(); i++) {
                    std::vector<int> temp = numbers;
                    temp.erase(temp.begin() + i); // Remove the i-th number
                    if (is_valid_sequence(temp)) {
                        valid = true;
                        break;
                    }
                }

                // If the sequence is valid, increment the safe counter
                if (valid) {
                    std:: cout << count << std::endl;
                    safe++;
                }}
        }

        //std::cout << "Safe sequences count: " << safe << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}

