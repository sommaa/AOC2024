#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <numeric>
#include <unordered_set>

class card {
private:
    int id;
    std::unordered_set<int> winning_numbers;
    std::vector<int> my_numbers;

public:
    card(int card_id, const std::vector<int>& winning, const std::vector<int>& my)
        : id(card_id)
        , my_numbers(my) {
        winning_numbers.reserve(winning.size());
        winning_numbers.insert(winning.begin(), winning.end());
    }

    ~card() = default;  // Use default destructor

    int get_id() const { return id; }
    const std::unordered_set<int>& get_winning_numbers() const { return winning_numbers; }
    const std::vector<int>& get_my_numbers() const { return my_numbers; }
};

class card_parser {
public:
    static card parse_line(const std::string& line) {
        size_t id_start = line.find_first_of("0123456789");
        size_t id_end = line.find(':', id_start);
        int card_id = std::stoi(line.substr(id_start, id_end - id_start)) - 1;

        size_t separator_pos = line.find('|');
        return card(
            card_id,
            parse_numbers(line.substr(id_end + 1, separator_pos - id_end - 1)),
            parse_numbers(line.substr(separator_pos + 1))
        );
    }

private:
    static std::vector<int> parse_numbers(const std::string& input) {
        std::vector<int> numbers;
        numbers.reserve(16); 
        
        std::istringstream iss(input);
        int num;
        while (iss >> num) {
            numbers.push_back(num);
        }
        return numbers;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false); 
    std::cin.tie(nullptr);

    std::string line;
    std::vector<card> cards;
    cards.reserve(256);  // Reserve space for typical input size

    // Read input
    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            cards.push_back(card_parser::parse_line(line));
        }
    }

    // Initialize scratchcard counts
    std::vector<int> num_scratchcards(cards.size(), 1);

    // Process cards
    for (size_t i = 0; i < cards.size(); ++i) {
        const auto& current_card = cards[i];
        const auto& winning_set = current_card.get_winning_numbers();
        
        // Count matches
        int matches = 0;
        for (int num : current_card.get_my_numbers()) {
            matches += winning_set.count(num);  // O(1) lookup
        }

        // Update future card counts
        int current_copies = num_scratchcards[i];
        size_t end_idx = std::min(i + matches + 1, cards.size());
        for (size_t j = i + 1; j < end_idx; ++j) {
            num_scratchcards[j] += current_copies;
        }
    }

    // Calculate sum using compiler optimization for accumulate
    std::cout << std::accumulate(num_scratchcards.begin(), num_scratchcards.end(), 0) << std::endl;
    return 0;
}
