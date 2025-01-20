#include <bits/stdc++.h>

class Modules {
private:
    enum class module_type { broadcaster, flip_flop, conjunction, output };

    struct module_info {
        module_type type;
        std::vector<std::string> destinations;
        bool state = false;                           // for flip-flops
        std::unordered_map<std::string, bool> inputs; // for conjunctions
    };

    std::unordered_map<std::string, module_info> modules;

public:
    void print_modules() {
        for (const auto &[name, info] : modules) {
            std::cout << name << " -> ";
            for (const auto &dest : info.destinations) {
                std::cout << dest << " ";
            }
            if (info.type == module_type::flip_flop) {
                std::cout << "state: " << info.state;
            } else if (info.type == module_type::conjunction) {
                std::cout << "inputs: ";
                for (const auto &[input, state] : info.inputs) {
                    std::cout << input << ": " << state << " ";
                }
            }
            std::cout << std::endl;
        }
    }

    void process_configuration(std::istream &input) {
        std::string line;

        // First pass: Create all modules
        while (std::getline(input, line)) {
            if (line.empty())
                continue;

            std::istringstream iss(line);
            std::string module_def;
            std::getline(iss, module_def, '-');

            // Remove spaces
            module_def.erase(
                remove_if(module_def.begin(), module_def.end(), isspace),
                module_def.end());

            // Get module name and type
            std::string name = module_def;
            module_type type;

            if (name == "broadcaster") {
                type = module_type::broadcaster;
            } else {
                if (name[0] == '%') {
                    type = module_type::flip_flop;
                    name = name.substr(1);
                } else if (name[0] == '&') {
                    type = module_type::conjunction;
                    name = name.substr(1);
                }
            }

            // Skip the ">" character
            std::string dummy;
            iss >> dummy;

            // Get destinations
            std::vector<std::string> destinations;
            std::string dest;
            while (std::getline(iss, dest, ',')) {
                // Remove spaces
                dest.erase(remove_if(dest.begin(), dest.end(), isspace),
                          dest.end());
                if (!dest.empty()) {
                    destinations.push_back(dest);
                }
            }

            modules[name] = {type, destinations};
        }

        // Second pass: Set up conjunction module inputs
        for (const auto &[name, info] : modules) {
            for (const auto &dest : info.destinations) {
                if (modules.count(dest) &&
                    modules[dest].type == module_type::conjunction) {
                    modules[dest].inputs[name] = false;
                }
            }
        }
    }

    std::pair<int64_t, int64_t> push_button(int n_push) {
        int64_t low = 0, high = 0;  // Changed to int64_t and initialized

        std::vector<std::string> broadcaster_targets;
        std::queue<std::tuple<std::string, std::string, int>> q;

        // Find broadcaster targets
        for (const auto &[name, info] : modules) {
            if (info.type == module_type::broadcaster) {
                broadcaster_targets = info.destinations;
                break;  // Only one broadcaster expected
            }
        }

        for (int i = 0; i < n_push; i++) {
            low++;  // Count button press as low pulse
            
            // Push broadcaster signals
            for (const auto &target : broadcaster_targets) {
                q.push({"broadcaster", target, 0});
            }

            while (!q.empty()) {
                auto [source, target, pulse] = q.front();
                q.pop();

                if (pulse == 0) {
                    low++;
                } else {
                    high++;
                }

                // Skip if target module doesn't exist
                if (modules.find(target) == modules.end()) {
                    continue;
                }

                if (modules[target].type == module_type::flip_flop) {
                    if (pulse == 0) {
                        modules[target].state = !modules[target].state;
                        int out_pulse = modules[target].state ? 1 : 0;
                        for (const auto &dest : modules[target].destinations) {
                            q.push({target, dest, out_pulse});
                        }
                    }
                } else if (modules[target].type == module_type::conjunction) {
                    modules[target].inputs[source] = pulse;
                    bool all_high = true;
                    for (const auto &[input, state] : modules[target].inputs) {
                        if (!state) {
                            all_high = false;
                            break;
                        }
                    }
                    int out_pulse = all_high ? 0 : 1;  // Fixed conjunction logic
                    for (const auto &dest : modules[target].destinations) {
                        q.push({target, dest, out_pulse});
                    }
                }
            }
        }
        return {low, high};
    }

    void clear() { modules.clear(); }
};

int main() {
    Modules modules;
    modules.process_configuration(std::cin);

    int n_push = 1000;
    auto [low, high] = modules.push_button(n_push);
    std::cout << (long long)low * high << std::endl;
    return 0;
}
