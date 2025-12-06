#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

class Day1 {
private:
    int64_t value;
    uint64_t count;

public:
    Day1() {
        value = 50;
        count = 0;
    }

    void turnLeft(uint64_t amount) {
        value -= amount;
        while (value < 0) {
            value += 100;
            if (value != 100-amount) {
                std::cout << "doing count + 1\n";
                count += 1;
            }
        }
        checkValue();
        std::cout << "Rotated left by " << amount << " new value is " << value << "\n";
    }

    void turnRight(uint64_t amount) {
        value += amount;
        while (value > 99) {
            value -= 100;
            if (value != 0) {
                std::cout << "doing count + 1\n";
                count += 1;
            }
        }
        checkValue();
        std::cout << "Rotated right by " << amount << " new value is " << value << "\n";
    }

    void checkValue() {
        if (value == 0) {
            count += 1;
        }
    }

    uint64_t getCount() {
        return count;
    }
};

int main() {
    Day1 day1;
    std::ifstream file("day1_final.input");

    if (!file.is_open()) {
        std::cerr << "Error opening the file! Exiting.\n";
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << "\n";
        if (line[0] == 'L') {
            day1.turnLeft(std::stoll(line.substr(1)));
        } else if (line[0] == 'R') {
            day1.turnRight(std::stoll(line.substr(1)));
        } else {
            std::cout << "what the sigma!\n";
        }
        std::cout << "\n";
    }

    std::cout << day1.getCount() << "\n";

    file.close();
    return 0;
}