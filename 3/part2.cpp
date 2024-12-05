#include <cctype> // for isdigit
#include <fstream>
#include <iostream>
#include <stdexcept>
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

int main() {
    // Import the input file
    std::vector<std::string> input = import_input("in.txt");

    int total_sum = 0;
    bool mul_enabled = true; // Tracks whether `mul` instructions are enabled

    // Loop through lines
    for (int i = 0; i < input.size(); i++) {
        std::string line = input[i];
        int line_sum = 0;

        size_t pos = 0;
        while (pos < line.size()) {
            if (line.substr(pos, 4) == "mul(") {
                // Process mul instruction if enabled
                if (mul_enabled) {
                    pos += 4; // Skip "mul("
                    std::string num1, num2;

                    // Extract num1
                    while (pos < line.size() && line[pos] != ',' &&
                           line[pos] != ')') {
                        if (isdigit(line[pos])) {
                            num1 += line[pos];
                        } else {
                            num1 = "";
                            break;
                        }
                        pos++;
                    }
                    pos++; // Skip ','

                    // Extract num2
                    while (pos < line.size() && line[pos] != ')') {
                        if (isdigit(line[pos])) {
                            num2 += line[pos];
                        } else {
                            num2 = "";
                            break;
                        }
                        pos++;
                    }
                    pos++; // Skip ')'

                    // Ensure valid numbers
                    if (!num1.empty() && !num2.empty()) {
                        int n1 = std::stoi(num1);
                        int n2 = std::stoi(num2);
                        line_sum += n1 * n2;
                    }
                } else {
                    // Skip
                    pos++;
                }
            } else if (line.substr(pos, 4) == "do()") {
                // Handle do() instruction
                mul_enabled = true;
                pos += 3;
            } else if (line.substr(pos, 7) == "don't()") {
                // Handle don't() instruction
                mul_enabled = false;
                pos += 7;
            } else {
                // Skip unrecognized characters
                pos++;
            }
        }
        total_sum += line_sum;
    }
    std::cout << "Total Sum: " << total_sum << std::endl;
    return 0;
}
