#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

std::unordered_map<std::string,
                   std::tuple<std::string, std::string, std::string>>
    circuit;

void load_circuit(
    const std::string &filename,
    std::unordered_map<std::string,
                       std::tuple<std::string, std::string, std::string>>
        &circuit) {
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("Can't open: " + filename);
    std::string line;
    std::regex gate_rx(R"((\w+)\s+(\w+)\s+(\w+)\s+->\s+(\w+))");
    std::smatch m;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;
        if (std::regex_match(line, m, gate_rx)) {
            circuit[m[4]] = {m[1], m[3], m[2]}; // Wire -> Gate details
        }
    }
}

std::string print_organized(const std::string &wire, int depth) {
    if (wire[0] == 'x' || wire[0] == 'y') {
        return std::string(depth, ' ') + wire;
    }

    auto [in1, in2, op] = circuit[wire];
    return std::string(depth, ' ') + op + " (" + wire + ")\n" +
           print_organized(in1, depth + 1) + "\n" +
           print_organized(in2, depth + 1);
}

int main() {
    // Load circuit at startup
    load_circuit("in.txt", circuit);

    std::string input;
    while (true) {
        // Display prompt
        std::cout << "\n> ";
        std::getline(std::cin, input);

        // Convert input to lowercase for command checking
        std::string command = input;
        std::transform(command.begin(), command.end(), command.begin(),
                       ::tolower);

        // Parse input for wire and depth
        std::istringstream iss(input);
        std::string wire_name;
        std::string depth_str;

        if (!(iss >> wire_name >> depth_str)) {
            std::cout
                << "Error: Invalid input format. Type 'help' for usage.\n";
            continue;
        }

        // Parse depth
        int depth;
        try {
            depth = std::stoi(depth_str);
        } catch (const std::exception &e) {
            std::cout
                << "Error: Invalid depth value. Please provide a number.\n";
            continue;
        }

        // Print result
        std::cout << print_organized(wire_name, depth) << std::endl;

        return 0;
    }
}
