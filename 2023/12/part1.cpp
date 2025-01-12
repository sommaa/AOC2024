#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// import the text from cin
void import_text(std::vector<std::string> &springs,
                 std::vector<std::vector<int>> &sequences) {
    std::string line;
    while (std::getline(std::cin, line)) {
        // split the line at the space into two parts
        // first part is the spring
        // second part is the sequence
        std::string spring = line.substr(0, line.find(" "));
        std::string sequence = line.substr(line.find(" ") + 1);
        springs.push_back(spring);
        std::vector<int> seq;
        // num divided by commas 1,10,2,3,...
        while (sequence.find(",") != std::string::npos) {
            seq.push_back(std::stoi(sequence.substr(0, sequence.find(","))));
            sequence = sequence.substr(sequence.find(",") + 1);
        }
        seq.push_back(std::stoi(sequence));
        sequences.push_back(seq);
    }
}

// count ways to fill the ? in the sequence,
// # is broken, . is not broken,
// the sequence is contiguous group of #s,
// that have to be divided at least once by a .
int count_ways(const std::string& spring, const std::vector<int>& sequence) {
    // If the string is empty, check if the sequence is also empty
    if (spring.empty()) {
        return sequence.empty() ? 1 : 0;
    }

    // If the sequence is empty, check if the string contains a '#'
    if (sequence.empty()) {
        return (spring.find("#") != std::string::npos) ? 0 : 1;
    }

    int result = 0;

    // Handle '.' or '?' at the current position
    if (spring[0] == '.' || spring[0] == '?') {
        result += count_ways(spring.substr(1), sequence);
    }

    // Handle '#' or '?' at the current position
    if (spring[0] == '#' || spring[0] == '?') {
        // Check if sequence[0] is valid for processing
        if (sequence[0] <= static_cast<int>(spring.size())
            && spring.substr(0, sequence[0]).find(".") == std::string::npos
            && (sequence[0] == static_cast<int>(spring.size()) || spring[sequence[0]] != '#')) {

            // Create a new sequence excluding the first element
            std::vector<int> remaining_sequence(sequence.begin() + 1, sequence.end());

            // Safely compute the remaining string
            std::string remaining_spring = (sequence[0] < static_cast<int>(spring.size()))
                                           ? spring.substr(sequence[0] + 1)
                                           : "";

            result += count_ways(remaining_spring, remaining_sequence);
        }
    }

    return result;
}

int main() {
    std::vector<std::string> springs;
    std::vector<std::vector<int>> sequences;
    import_text(springs, sequences);
    int res = 0;
    for (int i = 0; i < springs.size(); i++) {
        res += count_ways(springs[i], sequences[i]);
    }
    std::cout << res << std::endl;
    return 0;
}
