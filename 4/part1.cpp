#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype> // for isdigit
#include <stdexcept>

// trasform a input file rectangle of text into a 2D matrix of characters 
std::vector<std::vector<char>> import_input(const std::string& filename) {
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
    std::string word = "XMAS";
    int word_length = word.size();
    int count = 0; 

    int rows = input.size();
    int cols = input[0].size();


    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                
                int found = 0;
                for (int k = 0; k < word_length; k++) {
                    int nx = i + dx * k;
                    int ny = j + dy * k;
                    
                    // Check bounds
                    if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                        if (input[nx][ny] == word[k]) {
                            found++;
                        } else {
                            break; // Early exit if mismatch
                        }
                    } else {
                        break; // Out of bounds
                    }
                }
                
                if (found == word_length) {
                        count++;
                }
            }
        }
    }
    std::cout << "The word " << word << " appears " << count << " times." << std::endl;
}

 
    return 0;
}
