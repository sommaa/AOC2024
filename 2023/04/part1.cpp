#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <regex>
#include <iomanip>
#include <cmath>

class card {
private:
    int id;
    std::vector<int> winning_numbers;
    std::vector<int> my_numbers;

public:
    // Constructor
    card(int card_id, const std::vector<int>& winning, const std::vector<int>& my)
        : id(card_id), winning_numbers(winning), my_numbers(my) {}

    // Destructor
    ~card() {}  // Empty because std::vector handles its own cleanup

    // getters
    int get_id() const { return id; }
    const std::vector<int>& get_winning_numbers() const { return winning_numbers; }
    const std::vector<int>& get_my_numbers() const { return my_numbers; }
};

class card_parser {
public:
    static card parse_line(const std::string& line) {
        // extract card ID using regex
        std::regex card_pattern(R"(Card\s+(\d+):)");
        std::smatch matches;
        std::regex_search(line, matches, card_pattern);
        int card_id = std::stoi(matches[1]);

        // split into winning numbers and my numbers
        size_t colon_pos = line.find(':');
        size_t separator_pos = line.find('|');
        
        std::string winning_part = line.substr(colon_pos + 1, separator_pos - colon_pos - 1);
        std::string my_part = line.substr(separator_pos + 1);

        return card(card_id, parse_numbers(winning_part), parse_numbers(my_part));
    }

private:
    static std::vector<int> parse_numbers(const std::string& input) {
        std::vector<int> numbers;
        std::istringstream iss(input);
        int num;
        while (iss >> num) {
            numbers.push_back(num);
        }
        return numbers;
    }
};

int main() {
    std::string line;
    std::vector<card> cards;

    // read and parse each line
    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            cards.push_back(card_parser::parse_line(line));
        }
    }

    int sum = 0;
    for (const card& c : cards) {
        int local_sum = 0;
        std::vector<int> winning_numbers = c.get_winning_numbers();
        std::vector<int> my_numbers = c.get_my_numbers();
        for (int num : my_numbers) {
            if (std::find(winning_numbers.begin(), winning_numbers.end(), num) != winning_numbers.end()) {
                local_sum += 1;
            }
        }
        sum += std::pow(2, local_sum - 1);
    }

    std::cout << sum << std::endl;
    return 0;
}
