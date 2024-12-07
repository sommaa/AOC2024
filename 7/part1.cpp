#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Function to read the file and create a matrix of long long
std::vector<std::vector<long long>>
read_file_into_matrix(const std::string &filename) {
    std::ifstream f(filename);
    std::string s((std::istreambuf_iterator<char>(f)),
                  std::istreambuf_iterator<char>());
    std::vector<std::vector<long long>> matrix;

    std::istringstream stream(s);
    std::string line;

    while (std::getline(stream, line)) {
        // Remove ":" and "-" and split the line into numbers
        std::replace(line.begin(), line.end(), ':', ' ');
        std::replace(line.begin(), line.end(), '-', ' ');

        std::vector<long long> numbers;
        std::istringstream line_stream(line);
        long long num;
        while (line_stream >> num) {
            numbers.push_back(num);
        }
        matrix.push_back(numbers); // Add the numbers as a new row in the matrix
    }
    return matrix;
}

int main() {
    // Read the matrix from the file
    std::vector<std::vector<long long>> matrix =
        read_file_into_matrix("in.txt");

    long long result = 0;

    // Iterate over each row (equation)
    for (const auto &numbers : matrix) {
        std::vector<long long> num_list = numbers;

        bool valid = false;
        long long check_value = num_list[0];
        num_list.erase(
            num_list.begin()); // Remove the first number (the check value)

        // Iterate through all possible operator combinations
        int n = num_list.size();
        for (int combination = 0; combination < (1 << (n - 1)); combination++) {
            long long temp_result = num_list[0];
            for (int i = 1; i < n; i++) {
                int oper = (combination >> (i - 1)) & 1;
                if (oper == 1) {
                    temp_result = temp_result * num_list[i];
                } else {
                    temp_result = temp_result + num_list[i];
                }
            }

            if (temp_result == check_value) {
                valid = true;
                break;
            }
        }

        result += valid * check_value;
    }

    std::cout << result << std::endl;

    return 0;
}
