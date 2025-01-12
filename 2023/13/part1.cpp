#include <bits/stdc++.h>

std::vector<std::vector<std::string>> import_blocks() {
    std::vector<std::vector<std::string>> blocks{1};
    std::string line;
    while (std::getline(std::cin, line))
        line.empty() ? blocks.push_back({}) : blocks.back().push_back(line);
    return blocks;
}

size_t hash_vector(const std::vector<std::string> &v) {
    size_t hash = 0;
    for (const auto &s : v) {
        for (char c : s) {
            hash = hash * 0x100000001b3 + c;
        }
    }
    return hash;
}

int find_reflection(std::vector<std::string> block, bool transpose = false) {
    if (transpose) {
        std::vector<std::string> transposed(block[0].size(), std::string(block.size(), ' '));
        for (size_t i = 0; i < block.size(); ++i) {
            for (size_t j = 0; j < block[0].size(); ++j) {
                transposed[j][i] = block[i][j];
            }
        }
        block = transposed;
    }
    
    for (size_t i = 1; i < block.size(); ++i) {
        size_t max_size = std::min(i, block.size() - i);
        std::vector<std::string> top(block.begin() + i - max_size, block.begin() + i);
        std::vector<std::string> bottom(block.begin() + i, block.begin() + i + max_size);
        std::reverse(bottom.begin(), bottom.end());
        if (hash_vector(top) == hash_vector(bottom)) return i;
    }
    return 0;
}

int main() {
    int total = 0;
    for (const auto &block : import_blocks()) total += find_reflection(block) * 100 + find_reflection(block, true);
    std::cout << total << std::endl;
    return 0;
}
