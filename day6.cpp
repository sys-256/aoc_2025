#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
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
				switch (words[i][0]) {
					case '+':
						problems[i].first = Operation::ADD;
					case '*':
						problems[i].first = Operation::MULTIPLY;
					default:
						problems[i].first = Operation::UNKNOWN;
				}
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
			switch (pair.first) {
				case Operation::ADD:
					total += std::accumulate(pair.second.begin(), pair.second.end(), 0);
					break;
				case Operation::MULTIPLY:
					total += std::accumulate(pair.second.begin(), pair.second.end(), 1, std::multiplies<>{});
					break;
				case Operation::UNKNOWN:
					throw std::runtime_error("Unknown operation");
					break;
			}
		}
		return total;
	}
};

class Day6_part2 {
	private:
	// the long is for how many spaces after the operator
	// which is the same as the max length of a number
	std::vector<std::pair<Operation, long>> operators;
	std::vector<std::vector<int>> all_numbers; // -1 for a space

	void parse_numbers(std::string row) {
		std::vector<int> numbers;
		for (char c : row) {
			if (c == ' ') {
				numbers.push_back(-1);
			} else {
				numbers.push_back(c - '0');
			}
		}
		all_numbers.push_back(numbers);
	}

	void parse_operators(std::string row) {
		long length = 0;
		Operation current;
		for (char c : row) {
			switch (c) {
				case '+':
					add_operator(current, &length);
					current = Operation::ADD;
					break;
				case '*':
					add_operator(current, &length);
					current = Operation::MULTIPLY;
					break;
				case ' ':
					length += 1;
			}
		}
		length += 1;
		add_operator(current, &length);
	}

	void add_operator(Operation op, long *length) {
		if (*length != 0) {
			operators.push_back(std::make_pair(op, *length));
			*length = 0;
		}
	}

	public:
	void add_row(std::string row) {
		if (row[0] == '+' || row[0] == '*') {
			parse_operators(row);
		} else {
			parse_numbers(row);
		}
	}

	long get_total() {
		long index = 0;
		long total = 0;
		for (auto pair : operators) {
			// pair.second is amount of digits
			// all_numbers.size() is amount of rows
			std::vector<long> subnumbers;
			for (long i = index + pair.second - 1; i >= index; i--) {
				long subnumber = 0;
				long numbers_added = 0;
				for (int j = all_numbers.size()-1; j >= 0; j--) {
					if (all_numbers[j][i] >= 0) {
						subnumber += pow(10, numbers_added) * all_numbers[j][i];
						numbers_added += 1;
					}
				}
				subnumbers.push_back(subnumber);
			}
			switch (pair.first) {
				case Operation::ADD: {
					total += std::accumulate(subnumbers.begin(), subnumbers.end(), 0LL);
					break;
				}
				case Operation::MULTIPLY: {
					total += std::accumulate(subnumbers.begin(), subnumbers.end(), 1LL, std::multiplies<>{});
					break;
				}
				case Operation::UNKNOWN:
					throw std::runtime_error("Unknown operation");
					break;
			}
			index += pair.second + 1;
		}
		return total;
	}
};

int main() {
    Day6_part2 day6;
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