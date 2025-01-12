#include <bits/stdc++.h>

std::vector<std::vector<std::string>> import_blocks() {
    std::vector<std::vector<std::string>> blocks{1};
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            blocks.push_back({});
        } else {
            blocks.back().push_back(line);
        }
    }
    return blocks;
}

bool compare_blocks(std::vector<std::string> &block1, const std::vector<std::string> &block2) {
    int diff = 0;
    for (size_t i = 0; i < block1.size(); ++i) {
        for (size_t j = 0; j < block1[i].size(); ++j) {
            if (block1[i][j] != block2[i][j]) {
                diff++;
            }
            if (diff > 1) {
                return false;
            }
        }
    }
    return (diff == 1);
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
        
        if (compare_blocks(top, bottom)) {
            return i;
        }
    }
    return 0;
}

int main() {
    int total = 0;
    for (const auto &block : import_blocks()) {total += find_reflection(block) * 100 + find_reflection(block, true);}
    std::cout << total << std::endl;
    return 0;
}
