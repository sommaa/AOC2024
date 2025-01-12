#include <iostream>
#include <numeric>
#include <string>
#include <vector>

std::vector<std::vector<long long>> read_input() {
    std::vector<std::vector<long long>> input;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::vector<long long> row;
        long long start = 0;
        for (long long i = 0; i < line.size(); i++) {
            if (line[i] == ' ') {
                row.push_back(std::stoll(line.substr(start, i - start)));
                start = i + 1;
            }
        }
        row.push_back(std::stoll(line.substr(start, line.size() - start)));
        input.push_back(row);
    }
    return input;
}

int main() {
    std::vector<std::vector<long long>> input = read_input();
    std::vector<std::vector<long long>> diffs;
    long long sum = 0;

    for (long long i = 0; i < input.size(); i++) {
        std::vector<long long> current = input[i];
        diffs.push_back(current);

        while (true) {
            // Check if all elements are zero
            bool all_zeros = true;
            for (const auto &num : current) {
                if (num != 0) {
                    all_zeros = false;
                    break;
                }
            }
            if (all_zeros)
                break;

            // Calculate differences
            std::vector<long long> diff(current.size() - 1);
            for (size_t j = 0; j < current.size() - 1; j++) {
                diff[j] = current[j + 1] - current[j];
            }

            diffs.push_back(diff);
            current = diff;
        }

        // work backwards to find another number of the original sequence
        long long to_sum, current_back;
        current_back = diffs.back().back();
        for (long long j = diffs.size() - 1; j > 0; j--) {
            to_sum = diffs[j - 1].back();
            current_back += to_sum;
        }
        sum += current_back;
        diffs.clear();
    }
    std::cout << sum << std::endl;
    return 0;
}
