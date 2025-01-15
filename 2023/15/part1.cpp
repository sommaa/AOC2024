#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string& delim) {
    std::string line;
    std::getline(std::cin, line);
    std::vector<std::string> data;
    size_t pos = 0;
    while ((pos = line.find(delim)) != std::string::npos) {
        data.push_back(line.substr(0, pos));
        line.erase(0, pos + delim.length());
    }
    data.push_back(line);
    return data;
}

int hash(const std::string& s) {
    int h = 0;
    for (const auto& c : s) {
        h += int(c);
        h *= 17;
        h %= 256;
    }
    return h;
}

int main() {
    std::vector<std::string> data = split(",");
    int total = 0;
    for (const auto& s : data) {
        total += hash(s);
    }
    std::cout << total << std::endl;
    return 0;
}
