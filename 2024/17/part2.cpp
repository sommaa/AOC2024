#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<long long> parse_input(const std::string &filename) {
    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    std::vector<long long> numbers;
    std::string number;
    for (char ch : content) {
        if (isdigit(ch)) {
            number += ch;
        } else if (!number.empty()) {
            numbers.push_back(std::stoll(number));
            number.clear();
        }
    }
    if (!number.empty()) {
        numbers.push_back(std::stoll(number));
    }
    return numbers;
}

std::vector<long long> execute_program(const std::vector<long long> &program,
                                       long long register_a) {
    long long ip = 0, register_b = 0, register_c = 0;
    std::vector<long long> output;

    while (ip >= 0 && ip < program.size()) {
        long long operand_num = program[ip + 1];
        long long combo = std::vector<long long>{
            0, 1, 2, 3, register_a, register_b, register_c, 99999}[operand_num];

        switch (program[ip]) {
        case 0:
            register_a =
                static_cast<long long>(register_a / std::pow(2, combo));
            break;
        case 1:
            register_b ^= operand_num;
            break;
        case 2:
            register_b = combo % 8;
            break;
        case 3:
            ip = (register_a == 0) ? ip : operand_num - 2;
            break;
        case 4:
            register_b ^= register_c;
            break;
        case 5:
            output.push_back(combo % 8);
            break;
        case 6:
            register_b =
                static_cast<long long>(register_a / std::pow(2, combo));
            break;
        case 7:
            register_c =
                static_cast<long long>(register_a / std::pow(2, combo));
            break;
        }
        ip += 2;
    }
    return output;
}

long long find_register_a(const std::vector<long long> &program,
                          const std::vector<long long> &target,
                          long long register_a = 0, long long depth = 0) {
    if (depth == target.size()) {
        return register_a;
    }
    for (long long i = 0; i < 8; ++i) {
        std::vector<long long> output =
            execute_program(program, register_a * 8 + i);
        if (!output.empty() && output[0] == target[depth]) {
            long long result =
                find_register_a(program, target, register_a * 8 + i, depth + 1);
            if (result != 0) {
                return result;
            }
        }
    }
    return 0;
}

int main() {
    std::vector<long long> input = parse_input("in.txt");
    long long register_a = input[0], register_b = input[1],
              register_c = input[2];
    std::vector<long long> program(input.begin() + 3, input.end());

    std::vector<long long> target(program.rbegin(), program.rend());
    long long result = find_register_a(program, target);
    std::cout << "Register A: " << result << std::endl;

    return 0;
}
