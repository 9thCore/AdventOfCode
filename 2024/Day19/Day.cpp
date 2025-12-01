#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <unordered_set>
#include <unordered_map>
std::ifstream fin("input.txt");

typedef long long num;

struct data {
    typedef std::string towel;
    typedef std::string arrangement;
    typedef std::unordered_set<towel> towels;
    typedef std::vector<arrangement> arrangements;

    towels towelData;
    arrangements arrangementData;
};

void part1(data);
void part2(data);

int main()
{
    data input;

    std::regex regex("([a-z]+),? ?");
    std::smatch matches;

    std::string line;
    while (std::getline(fin, line)) {
        if (line.find(',') != line.npos) {
            while (std::regex_search(line, matches, regex)) {
                input.towelData.insert(matches[1]);
                line = matches.suffix();
            }
        }
        else if (!line.empty()) {
            input.arrangementData.push_back(line);
        }
    }

    part1(input);
    std::cout << "\n";
    part2(input);
}

std::unordered_map<std::string, bool> cachedResults;
bool correct(const data &pInput, const std::string &pArrangement) {
    if (pArrangement.empty()
        || pInput.towelData.find(pArrangement) != pInput.towelData.end()) {
        return true;
    }

    if (cachedResults.find(pArrangement) != cachedResults.end()) {
        return cachedResults[pArrangement];
    }

    for (num i = 0; i < pArrangement.size() - 1; i++) {
        std::string left = pArrangement.substr(0, i + 1);
        std::string right = pArrangement.substr(i + 1);
        if (correct(pInput, left) && correct(pInput, right)) {
            cachedResults[pArrangement] = true;
            return true;
        }
    }

    cachedResults[pArrangement] = false;
    return false;
}

void part1(data pInput) {
    num result = 0;
    for (auto &arrangement : pInput.arrangementData) {
        result += correct(pInput, arrangement);
    }
    std::cout << "part 1: " << result;
}

std::unordered_map<std::string, num> cachedPart2Results;
num arrangement_count(const data &pInput, const data::arrangement &pArrangement) {
    num sum = 0;

    if (cachedPart2Results.find(pArrangement) != cachedPart2Results.end()) {
        return cachedPart2Results[pArrangement];
    }

    for (auto &towel : pInput.towelData) {
        if (pArrangement == towel) {
            sum++;
        }
        else if (pArrangement.find(towel) == 0) {
            sum += arrangement_count(pInput, pArrangement.substr(towel.size()));
        }
    }

    cachedPart2Results[pArrangement] = sum;

    return sum;
}

void part2(data pInput) {
    num result = 0;
    for (auto &arrangement : pInput.arrangementData) {
        result += arrangement_count(pInput, arrangement);
    }
    std::cout << "part 2: " << result;
}