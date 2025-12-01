#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
std::ifstream fin("input.txt");
typedef std::vector<long long> history;

long long getnextinsequence(history sequence) {
	history newsequence;

	bool allzero = true;
	for (long long i = 1; i < sequence.size(); i++) {
		long long newvalue = sequence[i] - sequence[i - 1];
		newsequence.push_back(newvalue);

		allzero &= newvalue == 0;
	}

	if (allzero) {
		return sequence[0];
	}

	return sequence[sequence.size() - 1] + getnextinsequence(newsequence);
}

long long getprevinsequence(history sequence) {
	history newsequence;

	bool allzero = true;
	for (long long i = 1; i < sequence.size(); i++) {
		long long newvalue = sequence[i] - sequence[i - 1];
		newsequence.push_back(newvalue);

		allzero &= newvalue == 0;
	}

	if (allzero) {
		return sequence[0];
	}

	return sequence[0] - getprevinsequence(newsequence);
}

void part1(const std::vector<history>& histories) {
	long long sum = 0;

	for (history history : histories) {
		sum += getnextinsequence(history);
	}

	std::cout << "Part 1: " << sum << "\n";
}

void part2(const std::vector<history>& histories) {
	long long sum = 0;

	for (history history : histories) {
		sum += getprevinsequence(history);
	}

	std::cout << "Part 2: " << sum << "\n";
}

int main()
{
	std::vector<history> histories;
	std::string line;
	while (std::getline(fin, line)) {
		history history;

		long long x;
		std::istringstream s(line);
		while (s >> x) {
			history.push_back(x);
		}

		histories.push_back(history);
	}

	part1(histories);
	part2(histories);
}