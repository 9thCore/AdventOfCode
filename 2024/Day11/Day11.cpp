#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
std::ifstream fin("input.txt");

typedef long long num;
typedef std::unordered_map<num, num> data;

void part1(data);
void part2(data);

int main()
{
	data input;

	num x;
	while (fin >> x) {
		if (input.find(x) == input.end()) {
			input[x] = 0;
		}
		input[x]++;
	}

	part1(input);
	std::cout << "\n";
	part2(input);
}

int digit_count(num value) {
	int count = (value == 0);
	while (value) {
		count++;
		value /= 10;
	}
	return count;
}

num get_amount(const data &pInput) {
	num sum = 0;
	for (auto it = pInput.begin(), fin = pInput.end(); it != fin; it++) {
		sum += it->second;
	}
	return sum;
}

void simulate(data &pInput) {
	data newData;

	for (auto it = pInput.begin(), fin = pInput.end(); it != fin; it++) {
		num value = it->first, count = it->second;
		if (count < 1) {
			continue;
		}

		int cnt = digit_count(value);
		if (value == 0) {
			newData[1] += count;
		} else if (cnt % 2 == 0) {
			num pow10 = std::pow(10LL, cnt / 2);
			num first_half = value / pow10;
			num second_half = value % pow10;
			newData[first_half] += count;
			newData[second_half] += count;
		}
		else {
			num next_key = value * 2024;
			newData[next_key] += count;
		}
	}

	pInput = newData;
}

void simulate_for(data &pInput, int pSteps) {
	for (int i = 0; i < pSteps; i++) {
		simulate(pInput);
	}
}

void part1(data pInput) {
	simulate_for(pInput, 25);
	std::cout << "part 1: " << get_amount(pInput);
}

void part2(data pInput) {
	simulate_for(pInput, 75);
	std::cout << "part 2: " << get_amount(pInput);
}