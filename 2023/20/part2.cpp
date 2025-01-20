#include <bits/stdc++.h>

class Modules {
private:
    enum class module_type { broadcaster, flip_flop, conjunction, output };

    struct module_info {
        module_type type;
        std::string name;  // Added name field
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

            modules[name] = {type, name, destinations};  // Added name to struct initialization
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

    std::map<std::string, int64_t> push_button(const std::string &feed) {
        std::vector<std::string> broadcaster_targets;
        std::queue<std::tuple<std::string, std::string, int>> q;
        std::unordered_map<std::string, bool> first_seen;
        std::map<std::string, int64_t> cycle_lengths;
        int64_t presses = 0;

        // Get modules that feed into the target module
        std::vector<std::string> feed_sources = find_modules(feed);
        
        // Initialize tracking maps
        for (const auto &source : feed_sources) {
            first_seen[source] = false;
        }

        // Find broadcaster targets
        for (const auto &[name, info] : modules) {
            if (info.type == module_type::broadcaster) {
                broadcaster_targets = info.destinations;
                break;
            }
        }

        while (true) {
            presses++;
            
            // Push broadcaster signals
            for (const auto &target : broadcaster_targets) {
                q.push({"broadcaster", target, 0});
            }

            while (!q.empty()) {
                auto [source, target, pulse] = q.front();
                q.pop();

                // Skip if target doesn't exist
                if (modules.find(target) == modules.end()) {
                    continue;
                }

                // Check for high pulse to feed module
                if (target == feed && pulse == 1) {
                    if (!first_seen[source]) {
                        first_seen[source] = true;
                        cycle_lengths[source] = presses;
                    }

                    // If we've found all cycles, return
                    if (cycle_lengths.size() == feed_sources.size()) {
                        return cycle_lengths;
                    }
                }

                // Process module based on type
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
                    int out_pulse = all_high ? 0 : 1;
                    for (const auto &dest : modules[target].destinations) {
                        q.push({target, dest, out_pulse});
                    }
                }
            }
        }
    }

    std::vector<std::string> find_modules(const std::string &target) {
        std::vector<std::string> result;
        for (const auto &[name, info] : modules) {
            for (const auto &dest : info.destinations) {
                if (dest == target) {
                    result.push_back(name);
                }
            }
        }
        return result;
    }

    void clear() { modules.clear(); }
};

int main() {
    Modules modules;
    modules.process_configuration(std::cin);

    // Verify single input to rx
    std::string feed = modules.find_modules("rx")[0];
    
    // Get cycle lengths
    auto cycle_lengths = modules.push_button(feed);
    
    // Calculate least common multiple of all cycle lengths
    int64_t result = 1;
    for (const auto &[source, length] : cycle_lengths) {
        result = std::lcm(result, length);
    }
    
    std::cout << result << std::endl;
    return 0;
}
