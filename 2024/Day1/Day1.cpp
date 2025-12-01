#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
std::ifstream fin("input.txt");

void part1(std::vector<int>, std::vector<int>);
void part2(std::vector<int>, std::vector<int>);

int main()
{
	std::vector<int> firstList;
	std::vector<int> secondList;

	int x, y;
	while (fin >> x >> y) {
		firstList.push_back(x);
		secondList.push_back(y);
	}

	part1(firstList, secondList);
	std::cout << "\n";

	part2(firstList, secondList);
}

void part1(std::vector<int> pFirstList, std::vector<int> pSecondList) {
	std::sort(pFirstList.begin(), pFirstList.end());
	std::sort(pSecondList.begin(), pSecondList.end());
	unsigned long long sum = 0;

	// assume both lists are the same size
	for (int i = 0; i < pFirstList.size(); i++) {
		sum += abs(pFirstList[i] - pSecondList[i]);
	}

	std::cout << "part 1: " << sum;
}

void part2(std::vector<int> pFirstList, std::vector<int> pSecondList) {
	std::map<int, int> count;
	unsigned long long sum = 0;

	for (const int x : pSecondList) {
		count[x]++;
	}

	for (const int x : pFirstList) {
		sum += (unsigned long long)x * count[x];
	}

	std::cout << "part 2: " << sum;
}