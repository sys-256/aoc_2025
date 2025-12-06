#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

class Day3 {
    private:
    std::vector<long> max_joltages;

    void add_bank(std::vector<long> joltages, long n_batteries) {
        long prev_index = -1;
        long max_joltage = 0;

        for (int i = 0; i < n_batteries; i++) {
            long index = prev_index + 1; // Add 1 to index of previous battery
            for (long j = index + 1; j < joltages.size() - (n_batteries - i - 1); j++) {
                if (joltages[j] > joltages[index]) {
                    index = j;
                }
            }
            prev_index = index;
            max_joltage += pow(10, n_batteries - i - 1) * joltages[index];
        }

        max_joltages.push_back(max_joltage);
        std::cout << "Added max joltage " << max_joltage << "\n";
    }

    public:
    void add_bank(std::string bank) {
        std::cout << "Adding bank " << bank << "\n";
        std::vector<long> joltages;
        for (long i = 0; i < bank.length(); i++) {
            joltages.push_back(bank[i] - '0');
        }
        add_bank(joltages, 12);
    }

    long get_sum() {
        return std::reduce(max_joltages.begin(), max_joltages.end());
    }
};

int main() {
    Day3 day3;
    std::ifstream file("day3_final.input");

    if (!file.is_open()) {
        std::cerr << "Error opening the file! Exiting.\n";
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        day3.add_bank(line);
    }

    std::cout << day3.get_sum() << "\n";

    file.close();
    return 0;
}