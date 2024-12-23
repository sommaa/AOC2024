#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

// import the input file < in.txt function
std::vector<long long> read_input(std::string filename) {
    std::ifstream file(filename);
    std::vector<long long> input;
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            long long num;
            while (iss >> num) {
                input.push_back(num);
            }
        }
        file.close();
    }
    return input;
}

// simulate the program
long long simulate(long long num) {
    num = (num ^ (num * 64)) % 16777216;
    num = (num ^ (num / 32)) % 16777216;
    num = (num ^ (num * 2048)) % 16777216;
    return num;
}

int main(int argc, char *argv[]) {
    std::string filename = argv[1];
    std::vector<long long> input = read_input(filename);
    std::map<std::vector<int>, int> seq_count;

    for (int i = 0; i < input.size(); i++) {
        long long local = input[i];
        std::vector<long long> buyer = {local % 10};
        for (int j = 0; j < 2000; j++) {
            local = simulate(local);
            buyer.push_back(local % 10);
        }
        std::set<std::vector<int>> seen;
        for (int j = 0; j < buyer.size() - 4; j++) {
            int a = buyer[j];
            int b = buyer[j + 1];
            int c = buyer[j + 2];
            int d = buyer[j + 3];
            int e = buyer[j + 4];

            std::vector<int> diff = {b - a, c - b, d - c, e - d};
            if (seen.find(diff) != seen.end()) {
                continue;
            }
            seen.insert(diff);
            if (seq_count.find(diff) == seq_count.end()) {
                seq_count[diff] = 0;
            }
            seq_count[diff] += e;
        }
    }
    // find max value
    int max = 0;
    for (auto it = seq_count.begin(); it != seq_count.end(); it++) {
        if (it->second > max) {
            max = it->second;
        }
    }
    std::cout << "Max: " << max << std::endl;
}
