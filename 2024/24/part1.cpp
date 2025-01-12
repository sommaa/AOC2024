#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

class Gates_class {
    struct Gate { std::string in1, in2, op, out; };
    std::unordered_map<std::string, int> wires;
    std::vector<Gate> gates;

public:
    void add_wire(const std::string& w, int v) { wires[w] = v; }

    void add_gate(const std::string& in1, const std::string& in2, 
                 const std::string& op, const std::string& out) {
        gates.push_back({in1, in2, op, out});
    }

    int simulate(const std::string& wire) {
        if (auto it = wires.find(wire); it != wires.end()) return it->second;

        for (const auto& g : gates) {
            if (g.out == wire) {
                int l = simulate(g.in1);
                int r = simulate(g.in2);
                return wires[wire] = g.op == "AND" ? l & r : 
                                   g.op == "OR"  ? l | r : 
                                   g.op == "XOR" ? l ^ r :
                                   throw std::runtime_error("Unknown op: " + g.op);
            }
        }
        throw std::runtime_error("Wire not found: " + wire);
    }

    auto get_wires() const { return wires; }

    auto get_gates() const { return gates; }
};

void load_circuit(const std::string& filename, Gates_class& g) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Can't open: " + filename);

    std::string line;
    std::regex wire_rx(R"((\w+):\s*(\d+))"), gate_rx(R"((\w+)\s+(\w+)\s+(\w+)\s+->\s+(\w+))");
    std::smatch m;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (std::regex_match(line, m, wire_rx))
            g.add_wire(m[1], std::stoi(m[2]));
        else if (std::regex_match(line, m, gate_rx))
            g.add_gate(m[1], m[3], m[2], m[4]);
        else throw std::runtime_error("Bad format: " + line);
    }
}

int main() {
    Gates_class g;
    load_circuit("in.txt", g);

    for (const auto& gate : g.get_gates()) {
        g.simulate(gate.out);
    }

    std::vector<std::pair<std::string, int>> z_wires;
    for (const auto& [w, v] : g.get_wires()) {
        if (w[0] == 'z') z_wires.push_back({w, v});
    }

    std::sort(z_wires.begin(), z_wires.end(), 
             [](const auto& a, const auto& b) { return a.first > b.first; });

    std::string binary;
    for (const auto& [w, v] : z_wires) {
        binary += std::to_string(v & 1);
    }

    long long decimal = 0;
    for (char bit : binary) decimal = decimal * 2 + (bit - '0');
    std::cout << "Decimal value: " << decimal << '\n';
}
