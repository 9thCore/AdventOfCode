#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
std::ifstream fin("input.txt");

typedef std::vector<int> report;
typedef std::vector<report> data;

void part1(const data &);
void part2(const data &);

int main()
{
	data input;

	std::string line;
	int x;

	while (std::getline(fin, line)) {
		input.emplace_back();

		std::istringstream stream(line);
		while (stream >> x) {
			input.back().push_back(x);
		}
	}

	part1(input);
	std::cout << "\n";
	part2(input);
}

bool increasing(int a, int b) {
	return a < b;
}

bool standardCheck(int pShouldIncrease, int a, int b) {
	return increasing(a, b) == pShouldIncrease
		&& a != b
		&& std::abs(a - b) <= 3;
}

bool safe(const report &pReport) {
	if (pReport.size() < 2) {
		return true;
	}

	bool shouldIncrease = increasing(pReport.at(0), pReport.at(1));

	for (auto i = begin(pReport) + 1, e = end(pReport); i != e; i++) {
		int current = *i, previous = *(i - 1);

		if (!standardCheck(shouldIncrease, previous, current)) {
			return false;
		}
	}

	return true;
}

void part1(const data &pInput) {
	int count = 0;

	for (const auto &report : pInput) {
		count += safe(report);
	}

	std::cout << "part 1: " << count;
}

void part2(const data &pInput) {
	int count = 0;

	for (const auto &oldReport : pInput) {
		report newReport;

		// lol
		for (int i = 0; i < oldReport.size(); i++) {
			newReport.clear();

			for (int j = 0; j < oldReport.size(); j++) {
				if (i != j) {
					newReport.push_back(oldReport.at(j));
				}
			}

			if (safe(newReport)) {
				count++;
				break;
			}
		}
	}

	std::cout << "part 2: " << count;
}