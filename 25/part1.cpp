#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::vector<int>> key;
std::vector<std::vector<int>> lock;

void process_block(const std::vector<std::vector<char>> &block) {
    std::vector<int> column_counts(block[0].size(), -1);
    for (size_t col = 0; col < block[0].size(); ++col) {
        for (size_t row = 0; row < block.size(); ++row) {
            if (block[row][col] == '#') {
                column_counts[col]++;
            }
        }
    }
    bool isLock = true;
    bool isKey = true;

    for (char c : block[0]) {
        if (c != '#') {
            isLock = false;
            break;
        }
    }

    for (char c : block.back()) {
        if (c != '#') {
            isKey = false;
            break;
        }
    }

    if (isLock) {
        lock.push_back(column_counts);
    } else if (isKey) {
        key.push_back(column_counts);
    }
}

void read_file(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return;
    }

    std::vector<std::vector<char>> block;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) {
            if (!block.empty()) {
                process_block(block);
                block.clear();
            }
        } else {
            std::vector<char> row(line.begin(), line.end());
            block.push_back(row);
        }
    }

    if (!block.empty()) {
        process_block(block);
    }

    file.close();
}

bool check_lock_key(const std::vector<int> &key, const std::vector<int> &lock) {
    for (size_t i = 0; i < key.size(); ++i) {
        if (key[i] + lock[i] > 5) {
            return false;
        }
    }
    return true;
}

int main() {
    read_file("in.txt");

    // check all possible combinations
    int total = 0;
    for (const auto &k : key) {
        for (const auto &l : lock) {
            if (check_lock_key(k, l)) {
                total++;
            }
        }
    }

    std::cout << "Total: " << total << std::endl;
    return 0;
}
