#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <utility>
#include <sstream>
std::ifstream fin("input.txt");

// result, [num1, num2, num3, ...]
typedef std::pair<long long, std::vector<long long>> equation;
typedef std::vector<equation> data;

void part1(const data &);
void part2(const data &);

int main()
{
	data input;
	std::string line;

	while (std::getline(fin, line)) {
		std::regex matcher("(\\d+): ");
		std::smatch match;

		if (!std::regex_search(line, match, matcher)) {
			std::cout << "invalid input '" << line << "' ??";
			exit(1);
		}

		input.emplace_back();
		input.back().first = std::stoll(match[1]);

		std::istringstream param_input(match.suffix());

		int x;
		while (param_input >> x) {
			input.back().second.push_back(x);
		}
	}

	part1(input);
	std::cout << "\n";
	part2(input);
}

enum allowed_operators {
	ADDITION = 1,
	MULTIPLICATION = 2,
	CONCATENATION = 4
};

bool explore_possibilities(const equation &pEquation, allowed_operators pOp, int pI, long long pRunningTotal) {
	if (pI == 0) {
		return pRunningTotal == pEquation.second[0];
	}

	long long value = pEquation.second[pI];

	if (pOp & CONCATENATION) {
		long long valueCopy = value;
		long long runningTotalCopy = pRunningTotal;

		while (runningTotalCopy && valueCopy && runningTotalCopy % 10 == valueCopy % 10) {
			runningTotalCopy /= 10;
			valueCopy /= 10;
		}

		if (valueCopy == 0 && explore_possibilities(pEquation, pOp, pI - 1, runningTotalCopy)) {
			return true;
		}
	}

	if (pOp & MULTIPLICATION) {
		if (pRunningTotal % value == 0 && explore_possibilities(pEquation, pOp, pI - 1, pRunningTotal / value)) {
			return true;
		}
	}
	
	if (pOp & ADDITION) {
		return explore_possibilities(pEquation, pOp, pI - 1, pRunningTotal - value);
	}

	return false;
}

bool explore_possibilities(const equation &pEquation, allowed_operators pOp) {
	return explore_possibilities(pEquation, pOp, pEquation.second.size() - 1, pEquation.first);
}

void part1(const data &pInput) {
	long long sum = 0;

	for (const equation &equation : pInput) {
		sum += explore_possibilities(equation, (allowed_operators)(ADDITION | MULTIPLICATION)) * equation.first;
	}

	std::cout << "part 1: " << sum;
}

void part2(const data &pInput) {
	long long sum = 0;

	for (const equation &equation : pInput) {
		sum += explore_possibilities(equation, (allowed_operators)(ADDITION | MULTIPLICATION | CONCATENATION)) * equation.first;
	}

	std::cout << "part 1: " << sum;
}