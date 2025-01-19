#include <bits/stdc++.h>

struct Rule {
    char category;        // x, m, a, or s
    char comparison;      // < or >
    int value;
    std::string target;   // Where to go if rule matches
};

struct Workflow {
    std::string name;
    std::vector<Rule> rules;
    std::string default_target;  // Target when no rules match
};

struct Part {
    int x, m, a, s;
};

std::unordered_map<std::string, int> workflow_index;

std::pair<std::vector<Workflow>, std::vector<Part>> parse_input() {
    std::vector<Workflow> workflows;
    std::vector<Part> parts;
    std::string line;
    bool parsing_parts = false;

    // Regex patterns
    std::regex workflow_pattern(R"((\w+)\{(.+)\})");
    std::regex rule_pattern(R"((\w)([<>])(\d+):(\w+))");

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            parsing_parts = true;
            continue;
        }

        if (!parsing_parts) {
            // Parse workflow
            std::smatch workflow_match;
            if (std::regex_match(line, workflow_match, workflow_pattern)) {
                Workflow workflow;
                workflow.name = workflow_match[1];
                
                std::string rules_str = workflow_match[2];
                size_t pos = 0;
                while ((pos = rules_str.find(',')) != std::string::npos) {
                    std::string rule_str = rules_str.substr(0, pos);
                    rules_str.erase(0, pos + 1);

                    std::smatch rule_match;
                    if (std::regex_match(rule_str, rule_match, rule_pattern)) {
                        Rule rule;
                        rule.category = rule_match[1].str()[0];
                        rule.comparison = rule_match[2].str()[0];
                        rule.value = std::stoi(rule_match[3]);
                        rule.target = rule_match[4];
                        workflow.rules.push_back(rule);
                    }
                }
                // Handle default case (last element without condition)
                workflow.default_target = rules_str;
                workflows.push_back(workflow);
            }
        } else {
            // Parse part
            Part part;
            std::regex part_pattern(R"(\{x=(\d+),m=(\d+),a=(\d+),s=(\d+)\})");
            std::smatch part_match;
            if (std::regex_match(line, part_match, part_pattern)) {
                part.x = std::stoi(part_match[1]);
                part.m = std::stoi(part_match[2]);
                part.a = std::stoi(part_match[3]);
                part.s = std::stoi(part_match[4]);
                parts.push_back(part);
            }
        }
    }

    return {workflows, parts};
}

bool process_part(const Part& part, const std::vector<Workflow>& workflows) {
    std::string current_workflow = "in";
    int current_index = workflow_index[current_workflow];

    while (true) {
        const Workflow& workflow = workflows[current_index];
        bool matched = false;
        for (const auto& rule : workflow.rules) {
            int value = 0;
            switch (rule.category) {
                case 'x': value = part.x; break;
                case 'm': value = part.m; break;
                case 'a': value = part.a; break;
                case 's': value = part.s; break;
            }

            if ((rule.comparison == '<' && value < rule.value) ||
                (rule.comparison == '>' && value > rule.value)) {
                if (rule.target == "A") {
                    return true;
                } else if (rule.target == "R") {
                    return false;
                } else {
                    current_workflow = rule.target;
                    current_index = workflow_index[current_workflow];
                    matched = true;
                    break;
                }
            }
        }

        if (!matched) {
            if (workflow.default_target == "A") {
                return true;
            } else if (workflow.default_target == "R") {
                return false;
            } else {
                current_workflow = workflow.default_target;
                current_index = workflow_index[current_workflow];
            }
        }
    }
}

int main() {
    auto [workflows, parts] = parse_input();
    int total = 0;

    for (int i = 0; i < workflows.size(); i++) {
        workflow_index[workflows[i].name] = i;
    }

    for (const auto& part : parts) {
        if (process_part(part, workflows) == 1) {
            total += part.x + part.m + part.a + part.s;
        }
    }

    std::cout << total << std::endl;
    return 0;
}
