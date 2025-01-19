#include <bits/stdc++.h>

struct Rule {
    char category;   // x, m, a, or s
    char comparison; // < or >
    int value;
    std::string target; // Where to go if rule matches
};

struct Workflow {
    std::string name;
    std::vector<Rule> rules;
    std::string default_target; // Target when no rules match
};

std::unordered_map<std::string, int> workflow_index;

std::vector<Workflow> parse_input() {
    std::vector<Workflow> workflows;
    std::string line;

    // Regex patterns
    std::regex workflow_pattern(R"((\w+)\{(.+)\})");
    std::regex rule_pattern(R"((\w)([<>])(\d+):(\w+))");

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;  // Stop at empty line
        }
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
    }
    return workflows;
}

// recursive function to scan the ranges
long long count_ranges(const std::string& current_workflow, 
                      std::map<std::string, std::pair<int,int>> key_to_range, 
                      const std::vector<Workflow>& workflows) {
    if (current_workflow == "R") {
        return 0;
    }
    if (current_workflow == "A") {
        long long product = 1;
        for (const auto& [key, range] : key_to_range) {
            product *= (long long)(range.second - range.first + 1);
        }
        return product;
    }

    int current_index = workflow_index[current_workflow];
    const Workflow& workflow = workflows[current_index];

    long long total = 0;
    bool broken = false;
    auto current_ranges = key_to_range;  // Make a copy to modify

    for (const auto& rule : workflow.rules) {
        int low = current_ranges[std::string(1, rule.category)].first;
        int high = current_ranges[std::string(1, rule.category)].second;
        
        std::pair<int,int> true_range;
        std::pair<int,int> false_range;

        if (rule.comparison == '<') {
            true_range = {low, rule.value - 1};
            false_range = {rule.value, high};
        } else {  // '>'
            true_range = {rule.value + 1, high};
            false_range = {low, rule.value};
        }

        // Process matching range
        if (true_range.first <= true_range.second) {
            auto new_ranges = current_ranges;
            new_ranges[std::string(1, rule.category)] = true_range;
            total += count_ranges(rule.target, new_ranges, workflows);
        }

        // Update current ranges for next rule
        if (false_range.first <= false_range.second) {
            current_ranges[std::string(1, rule.category)] = false_range;
        } else {
            broken = true;
            break;
        }
    }

    if (!broken) {
        total += count_ranges(workflow.default_target, current_ranges, workflows);
    }
    return total;
}

int main() {
    auto workflows = parse_input();
    
    // fill the workflow_index map 
    for (int i = 0; i < workflows.size(); i++) {
        workflow_index[workflows[i].name] = i;
    }

    // fill the range of each key of xmas with 1 and 4000
    std::map<std::string, std::pair<int,int>> key_to_range;
    std::string keys = "xmas";
    for (char key : keys) {
        key_to_range[std::string(1, key)] = {1, 4000};
    }

    // Calculate and output the result
    long long result = count_ranges("in", key_to_range, workflows);
    std::cout << result << std::endl;

    return 0;
}
