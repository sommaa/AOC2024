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
            inst.direction = matches[1].str()[0];
            inst.steps = std::stoll(matches[2]);
            inst.color = matches[3];
            instructions.push_back(inst);
        }
    }
    
    return instructions;
}

long long calculateArea(const std::vector<Instruction>& instructions) {
    std::vector<std::pair<long long, long long>> vertices;
    long long x = 0, y = 0;
    long long perimeter = 0;
    
    // Collect vertices and calculate perimeter
    vertices.push_back({0, 0});
    for (const auto& inst : instructions) {
        perimeter += inst.steps;
        
        if (inst.direction == 'R') x += inst.steps;
        else if (inst.direction == 'L') x -= inst.steps;
        else if (inst.direction == 'U') y += inst.steps;
        else if (inst.direction == 'D') y -= inst.steps;
        
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
