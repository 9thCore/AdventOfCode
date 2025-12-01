#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
std::ifstream fin("input.txt");

struct race {
	long long time = 0;
	long long dist = 0;
};

long long getrange(race& r) {
	for (long long t = 0; t <= r.time; t++) {
		if ((r.time - t) * t > r.dist) {
			return t;
		}
	}
	return 0;
}

long long getpossibilities(race& r) {
	long long range = getrange(r);
	return r.time - range * 2 + 1;
}

void part1(std::vector<race> &races) {
	long long finalsum = 1;

	for (race& r : races) {
		finalsum *= getpossibilities(r);
	}

	std::cout << "Part 1: " << finalsum << "\n";
}

int get10pow(int n) {
	long long p = 1;
	while (p < n) { p *= 10; }
	return p;
}

void part2(std::vector<race>& races) {
	race realrace;

	for (race& r : races) {
		realrace.dist = realrace.dist * get10pow(r.dist) + r.dist;
		realrace.time = realrace.time * get10pow(r.time) + r.time;
	}

	std::cout << "Part 2: " << getpossibilities(realrace) << "\n";
}

int main()
{
	std::vector<race> races;

	std::string line;
	std::getline(fin, line);

	std::sregex_iterator regend = std::sregex_iterator();
	std::regex regnum = std::regex("\\b(\\d+)\\b");

	std::sregex_iterator regtimestart = std::sregex_iterator(line.begin(), line.end(), regnum);

	for (std::sregex_iterator i = regtimestart; i != regend; i++) {
		std::string str = (*i).str();
		races.push_back({ std::stoi(str), 0 });
	}

	std::getline(fin, line);

	std::sregex_iterator regdiststart = std::sregex_iterator(line.begin(), line.end(), regnum);

	int j = 0;
	for (std::sregex_iterator i = regdiststart; i != regend; i++) {
		std::string str = (*i).str();
		races[j++].dist = std::stoi(str);
	}

	part1(races);
	part2(races);
}