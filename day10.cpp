#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <numeric>
#include <regex>
#include <vector>

class Machine {
    private:
    std::vector<bool> goal_lights;
    std::vector<std::vector<int>> buttons;
    std::vector<int> goal_joltage;

	std::map<std::pair<int, int>, std::vector<std::vector<int>>> known_combinations_with;

	void util_combinations_without(std::vector<std::vector<int>> &result, std::vector<int> &temp, int n, int start, int k) {
		if (k == 0) {
			result.push_back(temp);
			return;
		}

		for (int i = start; i <= n; i++) {
			temp.push_back(i);
			util_combinations_without(result, temp, n, i+1, k-1);
			temp.pop_back();
		}
	}

	std::vector<std::vector<int>> get_combinations_without(int n, int k) {
		std::vector<std::vector<int>> combinations;
		std::vector<int> temp;
		util_combinations_without(combinations, temp, n, 1, k);
		return combinations;
	}

	void util_combinations_with(std::vector<std::vector<int>> &result, std::vector<int> &temp, int n, int start, int k) {
		if (k == 0) {
			result.push_back(temp);
			return;
		}

		for (int i = start; i <= n; i++) {
			temp.push_back(i);
			util_combinations_with(result,temp, n, i, k-1);
			temp.pop_back();
		}
	}

	std::vector<std::vector<int>> get_combinations_with(int n, int k) {
		if (auto result = known_combinations_with.find({n, k}); result != known_combinations_with.end()) {
			return result->second;
		}

		std::vector<std::vector<int>> combinations;
		std::vector<int> temp;
		util_combinations_with(combinations, temp, n, 1, k);
		known_combinations_with.insert({{n, k}, combinations});
		return combinations;
	}

	public:
	void add_lights(std::string lights) {
		// Start at +1, end at -1 to ignore the square brackets
		for (auto it = lights.begin() + 1; it != lights.end() - 1; it++) {
			goal_lights.push_back(*it == '#');
		}
	}

	void add_button(std::string option) {
		std::vector<int> option_vec;
		
		std::regex pattern("\\d+");
		auto digits_begin = std::sregex_iterator(option.begin(), option.end(), pattern);
		auto digits_end = std::sregex_iterator();
		for (auto it = digits_begin; it != digits_end; it++) {
			option_vec.push_back(std::stoi(it->str()));
		}
		
		buttons.push_back(option_vec);
	}

	void add_joltage(std::string joltage) {
		std::regex pattern("\\d+");
		auto digits_begin = std::sregex_iterator(joltage.begin(), joltage.end(), pattern);
		auto digits_end = std::sregex_iterator();
		for (auto it = digits_begin; it != digits_end; it++) {
			goal_joltage.push_back(std::stoi(it->str()));
		}
	}

	long get_fewest_lights_presses() {
		// Try single button first, then 2 buttons, up to the amount of buttons
		for (int k = 1; k <= buttons.size(); k++) {
			auto combinations = get_combinations_without(buttons.size(), k);
			for (auto combination : combinations) {
				// Combination is a vector of index of buttons to press
				std::vector<bool> lights(goal_lights.size(), false);
				for (auto button_index : combination) {
					auto button = buttons[button_index - 1];
					for (auto index : button) {
						lights[index] = !lights[index];
					}
				}
				if (lights == goal_lights) {
					return k;
				}
			}
		}

		// If we still haven't returned anything, it's impossible to
		// reach the goal lights
		throw std::runtime_error("Impossible to reach goal lights");
	}

	long get_fewest_joltage_presses() {
		// This approach is probably correct but it takes very very long to solve and has a memory leak
		// online people were using solvers and linear algebra so I decided to give up on this challenge

		// Try single button first, then 2 buttons, up to the amount of buttons
		for (int k = 1; k <= 1000; k++) { // We don't have a stop condition
			auto combinations = get_combinations_with(buttons.size(), k);
			for (auto combination : combinations) {
				// Combination is a vector of index of buttons to press
				std::vector<int> joltage(goal_joltage.size(), 0);
				for (auto button_index : combination) {
					auto button = buttons[button_index - 1];
					for (auto index : button) {
						joltage[index] += 1;
					}
				}
				if (joltage == goal_joltage) {
					return k;
				}
			}
		}

		// If we still haven't returned anything, it's impossible to
		// reach the goal joltage
		throw std::runtime_error("Impossible to reach goal joltage");
	}
};

class Day10 {
	private:
	std::vector<Machine> machines;

	public:
	void add_row(std::string row) {
		Machine machine;
		std::stringstream ss(row);
		std::string item;
		while (std::getline(ss, item, ' ')) {
			switch (item[0]) {
				case '[':
					machine.add_lights(item);
					break;

				case '(':
					machine.add_button(item);
					break;

				case '{':
					machine.add_joltage(item);
					break;
			
				default:
					break;
			}
		}
		machines.push_back(machine);
	}

	long get_fewest_lights_total() {
		return std::accumulate(machines.begin(), machines.end(), 0L, [](long sum, Machine& machine) {
			return sum + machine.get_fewest_lights_presses();
		});
	}

	long get_fewest_joltage_total() {
		return std::accumulate(machines.begin(), machines.end(), 0L, [](long sum, Machine& machine) {
			return sum + machine.get_fewest_joltage_presses();
		});
	}
};

int main() {
	Day10 day10;
	std::ifstream file("day10_final.input");

	if (!file.is_open()) {
		std::cerr << "Error opening the file! Exiting.\n";
		return 1;
	}


	// std::vector<std::vector<int>> combinations;
	// std::vector<int> temp;
	// Machine::get_combinations_with(combinations, temp, 3, 1, 4);

	std::string line;
	bool switched = false;
	while (std::getline(file, line)) {
		day10.add_row(line);
	}

	std::cout << day10.get_fewest_lights_total() << "\n";
	std::cout << day10.get_fewest_joltage_total() << "\n";

	file.close();
	return 0;
}