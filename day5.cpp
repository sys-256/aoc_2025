#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>

class Day5 {
    private:
    std::vector<std::vector<long>> fresh_ranges;
    std::vector<long> available_ids;

    public:
    void add_fresh(std::string range) {
        std::stringstream ss(range);
        std::string lower, upper;
        std::getline(ss, lower, '-');
        std::getline(ss, upper, '-');
        fresh_ranges.push_back({std::stoll(lower), std::stoll(upper)});
    }

    void add_available(std::string id) {
        available_ids.push_back(std::stoll(id));
    }

    long get_available_and_fresh() {
        long total = 0;
        for (long available_id : available_ids) {
            for (std::vector<long> range : fresh_ranges) {
                if (available_id >= range[0] && available_id <= range[1]) {
                    total += 1;
                    break;
                }
            }
        }
        return total;
    }

    long total_amount_fresh() {
        long total = 0;
        for (long i = 0; i < fresh_ranges.size(); i++) {
            std::vector<long> range = fresh_ranges[i];

            // Check if a previous range shares elements
            for (long j = 0; j < i; j++) {
                std::vector<long> prev_range = fresh_ranges[j];
                if (prev_range[0] <= range[0] && range[1] <= prev_range[1]) {
                    // This range has 0 new elements
                    range[1] = range[0] - 1;
                    break;
                } else if (prev_range[0] <= range[0] && prev_range[1] < range[1] && range[0] <= prev_range[1]) {
                    // This range only has new high elements
                    range[0] = prev_range[1] + 1;
                } else if (range[0] < prev_range[0] && range[1] <= prev_range[1] && prev_range[0] <= range[1]) {
                    // This range only has new low elements
                    range[1] = prev_range[0] - 1;
                } else if (range[0] < prev_range[0] && prev_range[1] < range[1]) {
                    // Example for this case: we already have [3,5], now we add [1,7]
                    // We'll add [1,2] and [6,7] to the fresh_ranges (to evaluate later)
                    // and do nothing now
                    fresh_ranges.push_back({range[0], prev_range[0] - 1});
                    fresh_ranges.push_back({prev_range[1] + 1, range[1]});
                    range[1] = range[0] - 1; // So we're adding 0
                    fresh_ranges[i][1] = range[0] - 1; // So this range isn't included for succeeding ranges
                }
                // If none of the cases applied, the ranges don't share any elements
                // and so we don't have to do anything
            }
            total += range[1] - range[0] + 1;
        }
        return total;
    }
};

int main() {
    Day5 day5;
    std::ifstream file("day5_final.input");

    if (!file.is_open()) {
        std::cerr << "Error opening the file! Exiting.\n";
        return 1;
    }

    std::string line;
    bool switched = false;
    while (std::getline(file, line)) {
        if (line == "" && !switched) {
            switched = true;
            continue;
        }

        if (!switched) {
            day5.add_fresh(line);
        } else {
            day5.add_available(line);
        }
    }

    std::cout << day5.total_amount_fresh() << "\n";

    file.close();
    return 0;
}