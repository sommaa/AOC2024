#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> read_input() {
    std::vector<std::string> input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input.push_back(line);
    }
    return input;
}

int main() {
    std::vector<std::string> input = read_input();
    int sum = 0;
    
    for (const std::string& line : input) {
        bool found_start = false;
        bool found_end = false;
        int first_digit = 0;
        int second_digit = 0;
        
        // Search through entire string
        for (int k = 0; k < line.size(); k++) {
            if (isdigit(line[k]) && !found_start) {
                first_digit = line[k] - '0';
                found_start = true;
                second_digit = first_digit; // Initialize second_digit in case only one digit exists
            }
            if (isdigit(line[k]) && found_start) {
                second_digit = line[k] - '0';
            }
        }
        
        int line_value = first_digit * 10 + second_digit;
        sum += line_value;
        std::cout << "first digit: " << first_digit << " second digit: " << second_digit << std::endl;
    }
    
    std::cout << "Total sum: " << sum << std::endl;
    return 0;
}
