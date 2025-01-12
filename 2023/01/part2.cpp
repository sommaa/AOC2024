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

int find_digit(const std::string& line, bool from_start) {
    static const std::vector<std::pair<std::string, int>> number_words = {
        {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5},
        {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}
    };

    if (from_start) {
        for (int i = 0; i < line.length(); i++) {
            if (isdigit(line[i])) {
                return line[i] - '0';
            }
            for (const auto& word : number_words) {
                if (line.substr(i, word.first.length()) == word.first) {
                    return word.second;
                }
            }
        }
    } else {
        for (int i = line.length() - 1; i >= 0; i--) {
            if (isdigit(line[i])) {
                return line[i] - '0';
            }
            for (const auto& word : number_words) {
                if (i + word.first.length() <= line.length() && 
                    line.substr(i, word.first.length()) == word.first) {
                    return word.second;
                }
            }
        }
    }
    return 0;
}

int main() {
    std::vector<std::string> input = read_input();
    int sum = 0;
    
    for (const std::string& line : input) {
        int first_digit = find_digit(line, true);
        int last_digit = find_digit(line, false);
        int value = first_digit * 10 + last_digit;
        sum += value;
        std::cout << line << ": " << value << std::endl;
    }
    
    std::cout << "Sum: " << sum << std::endl;
    return 0;
}
