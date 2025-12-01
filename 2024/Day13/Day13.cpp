#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <limits>
std::ifstream fin("input.txt");

typedef long long num;

struct point {
	num x, y;
};

struct machine {
	point aDelta, bDelta, prize;
};

typedef std::vector<machine> data;

void part1(const data &);
void part2(data);

int main()
{
	data input;

	std::string line;
	std::smatch match;
	std::regex regexes[3] = {
		std::regex("^Button A: X\\+(\\d+), Y\\+(\\d+)$"),
		std::regex("^Button B: X\\+(\\d+), Y\\+(\\d+)$"),
		std::regex("^Prize: X=(\\d+), Y=(\\d+)$")
	};

	while (std::getline(fin, line)) {
		if (std::regex_match(line, match, regexes[0])) {
			input.emplace_back();
			input.back().aDelta.x = std::stoi(match[1]);
			input.back().aDelta.y = std::stoi(match[2]);
		}
		else if (std::regex_match(line, match, regexes[1])) {
			input.back().bDelta.x = std::stoi(match[1]);
			input.back().bDelta.y = std::stoi(match[2]);
		}
		else if (std::regex_match(line, match, regexes[2])) {
			input.back().prize.x = std::stoi(match[1]);
			input.back().prize.y = std::stoi(match[2]);
		}
	}

	part1(input);
	std::cout << "\n";
	part2(input);
}

// i thought i could reuse this for part 2 lol
num calculate_sum(const data &pInput) {
	num sum = 0;

	for (const auto &machine : pInput) {
		num currentMinimum = LLONG_MAX;

		num aPressCount = 0;
		while (machine.aDelta.x * aPressCount < machine.prize.x
			&& machine.aDelta.y * aPressCount < machine.prize.y) {
			num remainderX = machine.prize.x - machine.aDelta.x * aPressCount;
			num remainderY = machine.prize.y - machine.aDelta.y * aPressCount;
			if (remainderX % machine.bDelta.x == 0 && remainderY % machine.bDelta.y == 0
				&& remainderX / machine.bDelta.x == remainderY / machine.bDelta.y) {
				num bPressCount = remainderX / machine.bDelta.x;
				num tokens = aPressCount * 3 + bPressCount;
				if (tokens < currentMinimum) {
					currentMinimum = tokens;
				}
			}

			aPressCount++;
		}

		if (currentMinimum != LLONG_MAX) {
			sum += currentMinimum;
		}
	}

	return sum;
}

void part1(const data &pInput) {
	std::cout << "part 1: " << calculate_sum(pInput);
}

void part2(data pInput) {
	num sum = 0;

	for (auto &arcade : pInput) {
		arcade.prize.x += 10000000000000;
		arcade.prize.y += 10000000000000;

		// timesB (number of times to push B) = (XaYp - YaXp) / (XaYb - YaXb)
		// where: Xa, Ya -> delta of an A push
		// Xp, Yp -> prize x/y pos
		// Xb, Ya -> delta of a B push

		num timesBNumerator = arcade.aDelta.x * arcade.prize.y - arcade.aDelta.y * arcade.prize.x;
		num timesBDenominator = arcade.aDelta.x * arcade.bDelta.y - arcade.aDelta.y * arcade.bDelta.x;

		if (timesBNumerator % timesBDenominator != 0) {
			continue;
		}

		num bPush = timesBNumerator / timesBDenominator;
		num remainderX = arcade.prize.x - bPush * arcade.bDelta.x;
		num remainderY = arcade.prize.y - bPush * arcade.bDelta.y;

		if (remainderX % arcade.aDelta.x != 0 || remainderY % arcade.aDelta.y != 0
			|| remainderX / arcade.aDelta.x != remainderY / arcade.aDelta.y) {
			continue;
		}

		num aPush = remainderX / arcade.aDelta.x;
		sum += aPush * 3 + bPush;
	}

	std::cout << "part 2: " << sum;
}