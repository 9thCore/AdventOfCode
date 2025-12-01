#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <regex>
#include <vector>
#include <unordered_set>
#include <unordered_map>
std::ifstream fin("input.txt");

// lol
constexpr int MAX_PAGE = 100;

// pageOrder[former][latter] = 1 if former should go before latter, 0 otherwise
typedef std::array<std::array<bool, MAX_PAGE>, MAX_PAGE> pageOrder;
typedef std::vector<int> printingOrder;
typedef std::vector<printingOrder> allPrintingOrders;

void part1(const pageOrder &, const allPrintingOrders &);
void part2(const pageOrder &, allPrintingOrders);

int main()
{
	pageOrder pageData = { 0 };
	allPrintingOrders printData;

	std::regex firstSection(R"(^(\d+)\|(\d+)$)");
	std::regex secondSection(R"(^([^,]+),?)");
	std::smatch matcher;

	std::string line;

	bool section = 0;
	while (std::getline(fin, line)) {
		if (line.size() < 1) {
			section = 1;
			continue;
		}

		if (section == 0 && std::regex_match(line, matcher, firstSection)) {
			int first = std::stoi(matcher[1].str()), second = std::stoi(matcher[2].str());

			pageData[first][second] = 1;
		} else if (section == 1) {
			printData.emplace_back();

			while (std::regex_search(line, matcher, secondSection)) {
				printData.back().push_back(std::stoi(matcher[1].str()));
				line = matcher.suffix().str();
			}
		}
	}

	part1(pageData, printData);
	std::cout << "\n";
	part2(pageData, printData);
}

bool rightOrderPrint(const pageOrder &pPageOrder, const printingOrder &pPrintingOrder) {
	std::unordered_set<int> previousValues;

	for (const int value : pPrintingOrder) {
		for (int i = 0; i < MAX_PAGE; i++) {
			// 'i' should have appeared after 'value', but it appeared before
			// therefore, incorrect order
			if (pPageOrder[value][i] && previousValues.find(i) != previousValues.end()) {
				return false;
			}
		}
		previousValues.insert(value);
	}

	return true;
}

void part1(const pageOrder &pPageOrder, const allPrintingOrders &pPrintingOrders) {
	int sum = 0;

	for (const auto &line : pPrintingOrders) {
		if (rightOrderPrint(pPageOrder, line)) {
			sum += line[line.size() / 2];
		}
	}

	std::cout << "part 1: " << sum;
}

bool findBadOrder(const pageOrder &pPageOrder, const printingOrder &pPrintingOrder, int &pFormerErronousIndex, int &pLatterErronousIndex) {
	std::unordered_map<int, int> previousValues;

	int idx = 0;
	for (const int &value : pPrintingOrder) {
		for (int i = 0; i < MAX_PAGE; i++) {
			// see comment in rightOrderPrint
			if (pPageOrder[value][i] && previousValues.find(i) != previousValues.end()) {
				pFormerErronousIndex = previousValues[i];
				pLatterErronousIndex = idx;
				return true;
			}
		}

		previousValues[value] = idx;
		idx++;
	}
	
	return false;
}

void part2(const pageOrder &pPageOrder, allPrintingOrders pPrintingOrders) {
	int sum = 0;

	for (auto &line : pPrintingOrders) {
		int formerIdx = -1, latterIdx = -1;
		while (findBadOrder(pPageOrder, line, formerIdx, latterIdx)) {
			std::swap(line[formerIdx], line[latterIdx]);
		}

		if (formerIdx != -1 && latterIdx != -1) {
			sum += line[line.size() / 2];
		}
	}

	std::cout << "part 2: " << sum;
}