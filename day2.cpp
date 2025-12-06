#include <cstdint>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

class Day2 {
    private:
    std::vector<uint64_t> invalid_ids;

    bool validateID(std::string str_id, uint64_t n) {
        size_t str_id_length = str_id.length();
        if (str_id_length % n != 0) {
            // Invalid IDs are divisible by the length of the substring
            return true;
        }
        for (int i = 0; i < n; i++) {
            // i is index within the substring
            for (int j = 0; j < str_id_length / n - 1; j++) {
                // j is the substring we are looking at
                // so for 123412341234, i is [0, 4), j is [0, 3)
                if (str_id[i+j*n] != str_id[i+(j+1)*n]) {
                    return true;
                }
            }
        }
        return false;
    }

    bool validateID(uint64_t id) {
        std::string str_id = std::to_string(id);
        // n is substring lenth
        for (int n = 1; n < str_id.length() / 2 + 1; n++) {
            if (!validateID(str_id, n)) {
                return false;
            }
        }
        return true;
    }

    void validate_range(uint64_t lower, uint64_t upper) {
        for (uint64_t i = lower; i <= upper; i++) {
            if (!validateID(i)) {
                invalid_ids.push_back(i);
            }
        }
    }

    void validate_range(std::string input) {
        std::stringstream ss(input);
        std::string lower, upper;
        std::getline(ss, lower, '-');
        std::getline(ss, upper, '-');
        validate_range(std::stoll(lower), std::stoll(upper));
    }

    public:
    void add_ranges(std::string input) {
        std::stringstream ss(input);
        std::string range;
        while (std::getline(ss, range, ',')) {
            validate_range(range);
        }
    }

    uint64_t get_sum() {
        uint64_t sum = 0;
        for (int i = 0; i < invalid_ids.size(); i++) {
            sum += invalid_ids[i];
        }
        return sum;
    }
};

int main() {
    Day2 day2;
    std::ifstream file("day2_final.input");

    if (!file.is_open()) {
        std::cerr << "Error opening the file! Exiting.\n";
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        day2.add_ranges(line);
    }

    std::cout << day2.get_sum() << "\n";

    file.close();
    return 0;
}