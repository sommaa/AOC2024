#include <cctype> // for isdigit
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// trasform a input file rectangle of text into a 2D matrix of characters
std::vector<std::vector<char>> import_input(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file " + filename);
    }

    std::vector<std::vector<char>> input;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) { // Skip empty lines
            std::vector<char> row;
            for (char c : line) {
                row.push_back(c);
            }
            input.push_back(row);
        }
    }
    return input;
}

int main() {
    // Import the input file
    std::vector<std::vector<char>> input = import_input("in.txt");
    std::string word = "MAS";
    std::string word_reverse = "SAM";
    int word_length = word.size();
    int count = 0;

    int rows = input.size();
    int cols = input[0].size();

    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            // found mas shaped in an X pattern
            if (input[i][j] == 'A') {
                int found = 0;
                std::string diagonalNW_SE = "";
                std::string diagonalSW_NE = "";

                // fill NW-SE diagonal string
                diagonalNW_SE += input[i - 1][j - 1];
                diagonalNW_SE += input[i][j];
                diagonalNW_SE += input[i + 1][j + 1];
                // fill SW-NE diagonal string
                diagonalSW_NE += input[i + 1][j - 1];
                diagonalSW_NE += input[i][j];
                diagonalSW_NE += input[i - 1][j + 1];

                if (diagonalNW_SE == word || diagonalNW_SE == word_reverse) {
                    found++;
                }
                if (diagonalSW_NE == word || diagonalSW_NE == word_reverse) {
                    found++;
                }
                if (found == 2) {
                    count++;
                }
            }
        }
    }
    std::cout << "Count: " << count << std::endl;
    return 0;
}
