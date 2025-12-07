#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

enum Operation {
	ADD,
	MULTIPLY,
	UNKNOWN,
};

class Day6 {
	private:
	std::vector<std::pair<Operation, std::vector<long>>> problems;

	std::vector<std::string> parse_row(std::string row) {
		std::vector<std::string> words;
		std::string buf;
		for (char c : row) {
			if (c != ' ') {
				buf += c;
			} else {
				if (buf != "") {
					words.push_back(buf);
					buf = "";
				}
			}
		}
		if (buf != "") {
			words.push_back(buf);
			buf = "";
		}
		return words;
	}

	public:
	void add_row(std::string row) {
		std::vector<std::string> words = parse_row(row);

		bool final_row = words[0] == "+" || words[0] == "*";
		for (long i = 0; i < words.size(); i++) {
			if (final_row) {
				// Characters is the last row, so we assume all columns have been created
				problems[i].first = words[i] == "+" ? Operation::ADD : Operation::MULTIPLY;
			} else {
				if (i >= problems.size()) {
					problems.push_back({Operation::UNKNOWN, {std::stol(words[i])}});
				} else {
					problems[i].second.push_back(std::stol(words[i]));
				}
			}
		}
	}

	long get_total() {
		long total = 0;
		for (auto pair : problems) {
			long result;
			if (pair.first == Operation::ADD) {
				result = 0;
				for (long x : pair.second) {
					result += x;
				}
			} else {
				result = 1;
				for (long x : pair.second) {
					result *= x;
				}
			}
			total += result;
		}
		return total;
	}
};

int main() {
    Day6 day6;
    std::ifstream file("day6_final.input");

    if (!file.is_open()) {
        std::cerr << "Error opening the file! Exiting.\n";
        return 1;
    }

    std::string line;
    bool switched = false;
    while (std::getline(file, line)) {
        day6.add_row(line);
    }

    std::cout << day6.get_total() << "\n";

    file.close();
    return 0;
}