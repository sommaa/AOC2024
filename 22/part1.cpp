#include <fstream>
#include <iostream>
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
long long simulate(long long num, long long n_sim) {
    for (long long i = 0; i < n_sim; i++) {
        num = (num ^ (num * 64)) % 16777216;
        num = (num ^ (num / 32)) % 16777216;
        num = (num ^ (num * 2048)) % 16777216;
    }
    return num;
}

int main(int argc, char *argv[]) {
    std::string filename = argv[1];
    std::vector<long long> input = read_input(filename);
    long long total = 0;
    for (long long i = 0; i < input.size(); i++) {
        total += simulate(input[i], 2000);
    }

    std::cout << "Total: " << total << std::endl;
    return 0;
}
