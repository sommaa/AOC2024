#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct Bid {
    std::string hand;
    int bidAmount;
    int rank;
};

int get_card_value(char c) {
    static const std::map<char, int> values = {
        {'J', 1}, {'2', 2}, {'3', 3},  {'4', 4},  {'5', 5},  {'6', 6}, {'7', 7},
        {'8', 8}, {'9', 9}, {'T', 10}, {'Q', 11}, {'K', 12}, {'A', 13}};
    return values.at(c);
}

int rank_hand(const std::string &hand) {
    std::map<char, int> counts;
    int num_jokers = 0;
    
    // Count jokers and other cards separately
    for (char c : hand) {
        if (c == 'J') {
            num_jokers++;
        } else {
            counts[c]++;
        }
    }
    
    // Handle all jokers case
    if (num_jokers == 5) {
        return 7;  // Five of a kind
    }
    
    // Get frequencies of non-joker cards
    std::vector<int> frequencies;
    for (const auto &pair : counts) {
        frequencies.push_back(pair.second);
    }
    
    // Sort frequencies in descending order
    std::sort(frequencies.begin(), frequencies.end(), std::greater<int>());
    
    // Add jokers to the highest frequency
    if (!frequencies.empty()) {
        frequencies[0] += num_jokers;
    }
    
    // Determine hand type
    if (frequencies[0] >= 5) return 7;  // Five of a kind
    if (frequencies[0] == 4) return 6;  // Four of a kind
    if (frequencies[0] == 3 && frequencies.size() > 1 && frequencies[1] == 2) return 5;  // Full house
    if (frequencies[0] == 3) return 4;  // Three of a kind
    if (frequencies[0] == 2 && frequencies.size() > 1 && frequencies[1] == 2) return 3;  // Two pair
    if (frequencies[0] == 2) return 2;  // One pair
    return 1;  // High card
}

bool operator<(const Bid &lhs, const Bid &rhs) {
    if (lhs.rank != rhs.rank) {
        return lhs.rank < rhs.rank;
    }
    
    for (int i = 0; i < 5; i++) {
        int lval = get_card_value(lhs.hand[i]);
        int rval = get_card_value(rhs.hand[i]);
        if (lval != rval) {
            return lval < rval;
        }
    }
    return false;
}

std::vector<Bid> read_bids() {
    std::vector<Bid> bids;
    std::string line;
    
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;
        
        std::istringstream stream(line);
        Bid bid;
        if (stream >> bid.hand >> bid.bidAmount) {
            bid.rank = rank_hand(bid.hand);
            bids.push_back(bid);
        } else {
            std::cerr << "Invalid input format: " << line << std::endl;
        }
    }
    return bids;
}

int main() {
    std::vector<Bid> bids = read_bids();
    std::sort(bids.begin(), bids.end());
    
    long long sum = 0;
    for (int i = 0; i < bids.size(); i++) {
        sum += static_cast<long long>(bids[i].bidAmount) * (i + 1);
    }
    
    std::cout << sum << std::endl;
    return 0;
}
