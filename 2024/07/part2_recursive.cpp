#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

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

// recursive function to calculate the result
bool calculate(std::vector<long long> &num_list, long long check_value,
               long long temp_result, int i) {
    if (i == num_list.size()) {
        return temp_result == check_value;
    }
    if (temp_result > check_value) {
        return false;
    }
    return calculate(num_list, check_value, temp_result + num_list[i], i + 1) ||
           calculate(num_list, check_value, temp_result * num_list[i], i + 1) ||
           calculate(num_list, check_value, temp_result * pow(10, std::to_string(num_list[i]).size()) + num_list[i], i + 1);
}

int main() {
    // Read the matrix from the file
    std::vector<std::vector<long long>> matrix =
        read_file_into_matrix("in.txt");

    long long result = 0;
    bool valid;
    // Iterate over each row (equation)
    for (const auto &numbers : matrix) {
        std::vector<long long> num_list = numbers;

        long long check_value = num_list[0];
        num_list.erase(
            num_list.begin()); // Remove the first number (the check value)

        valid = calculate(num_list, check_value, 0, 0);
        result += valid * check_value;
    }
    std::cout << result << std::endl;
    return 0;
}
