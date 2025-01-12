#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// import input from file in 2 blocks
// separated by a blank line
std::pair<std::vector<std::string>, std::vector<std::string>> read_input() {
    std::vector<std::string> block1, block2;
    std::string line;
    while (std::getline(std::cin, line) && !line.empty()) {
        block1.push_back(line);
    }
    while (std::getline(std::cin, line)) {
        block2.push_back(line);
    }

    // block1 strigs separated by ", " to vector
    std::vector<std::string> stripes;
    std::string stripe;
    for (const auto &line : block1) {
        for (const auto &c : line) {
            if (c == ',') {
                stripes.push_back(stripe);
                stripe.clear();
            } else if (c != ' ') {
                stripe.push_back(c);
            }
        }
        stripes.push_back(stripe);
        stripe.clear();
    }

    // block2 strings on each line to vector
    std::vector<std::string> towels;
    for (const auto &line : block2) {
        towels.push_back(line);
    }

    return {stripes, towels};
}

// recursive function to find if the towel can be made
// from the stripes
long long number_of_towels(const std::vector<std::string> &stripes,
                           const std::string &towel,
                           std::unordered_map<std::string, long long> &memo,
                           long long min_size) {
    if (towel.empty()) {
        return 1; // Base case: empty towel can always be made
    }
    if (memo.find(towel) != memo.end()) {
        return memo[towel];
    }
    long long result = 0;
    // Ensure towel size is large enough for at least one stripe
    for (const auto &stripe : stripes) {
        if (towel.size() >= min_size &&
            towel.substr(0, stripe.size()) == stripe) {
            result += number_of_towels(stripes, towel.substr(stripe.size()),
                                       memo, min_size);
            memo[towel] = result;
        }
    }
    return result;
}

std::unordered_map<std::string, long long> memo;

int main() {
    auto [stripes, towels] = read_input();

    long long result = 0;
    long long min_size = 1000;

    for (const auto &stripe : stripes) {
        min_size = std::min(min_size, (long long)stripe.size());
    }

    for (const auto &towel : towels) {
        result += number_of_towels(stripes, towel, memo, min_size);
    }

    std::cout << result << std::endl;

    return 0;
}
