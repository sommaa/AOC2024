#include <bits/stdc++.h>

struct Instruction {
    char direction;
    long long steps;
    std::string color;
};

std::vector<Instruction> parseInstructions() {
    std::vector<Instruction> instructions;
    std::string line;
    
    std::regex pattern(R"(([RDLU])\s+(\d+)\s+\(#([a-fA-F0-9]{6})\))");
    
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;
        
        std::smatch matches;
        if (std::regex_search(line, matches, pattern)) {
            Instruction inst;
            std::string hex = matches[3];
            // Take first 5 characters of hex and convert to decimal
            std::string lengthHex = hex.substr(0, 5);
            inst.steps = std::stoll(lengthHex, nullptr, 16);  // Convert hex to decimal
            inst.direction = matches[1].str()[0];  // We'll ignore this for part 2
            inst.color = hex;
            instructions.push_back(inst);
        }
    }
    
    return instructions;
}

// Convert the last digit of hex to direction
char getDirectionFromHex(char lastDigit) {
    switch(lastDigit) {
        case '0': return 'R';
        case '1': return 'D';
        case '2': return 'L';
        case '3': return 'U';
        default: return 'R';  // Default case, shouldn't occur with valid input
    }
}

long long calculateArea(const std::vector<Instruction>& instructions) {
    std::vector<std::pair<long long, long long>> vertices;
    long long x = 0, y = 0;
    long long perimeter = 0;
    
    // Collect vertices and calculate perimeter
    vertices.push_back({0, 0});
    for (const auto& inst : instructions) {
        // Get real direction from last hex digit
        char realDirection = getDirectionFromHex(inst.color[5]);
        perimeter += inst.steps;
        
        if (realDirection == 'R') x += inst.steps;
        else if (realDirection == 'L') x -= inst.steps;
        else if (realDirection == 'U') y += inst.steps;
        else if (realDirection == 'D') y -= inst.steps;
        
        vertices.push_back({x, y});
    }
    
    // Calculate area using shoelace formula
    long long area = 0;
    for (size_t i = 0; i < vertices.size() - 1; i++) {
        area += vertices[i].first * vertices[i + 1].second;
        area -= vertices[i].second * vertices[i + 1].first;
    }
    
    // Add last vertex to first vertex
    area += vertices.back().first * vertices[0].second;
    area -= vertices.back().second * vertices[0].first;
    
    // Take absolute value and divide by 2
    area = std::abs(area) / 2;
    
    // Add half of perimeter + 1 (Pick's theorem adjustment)
    return area + perimeter / 2 + 1;
}

int main() {
    auto instructions = parseInstructions();
    long long totalArea = calculateArea(instructions);
    std::cout << totalArea << std::endl;
    return 0;
}
