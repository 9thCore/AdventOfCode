#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
std::ifstream fin("input.txt");

typedef long long num;

struct data {
    struct computer {
        struct hasher {
            std::size_t operator()(const computer &pComputer) const {
                return pComputer.hash;
            }
        };

        const std::string identifier;
        std::size_t hash;

        computer(const std::string &pIdentifier) : identifier(pIdentifier) {
            hash = (std::size_t)(identifier[0] - 'a') * 27 + (std::size_t)(identifier[1] - 'a');
        }

        bool operator==(const computer &pRHS) const {
            return hash == pRHS.hash;
        }
    };

    typedef std::unordered_set<computer, computer::hasher> connections;
    typedef std::unordered_map<computer, connections, computer::hasher> lan;

    lan lanParty;
    connections computers;

    void add_connection(const computer &pKey, const computer &pValue) {
        computers.insert(pKey);
        computers.insert(pValue);

        lanParty.try_emplace(pKey);
        lanParty[pKey].insert(pValue);
    }

    bool connected(const computer &pFirst, const computer &pSecond) {
        return lanParty.find(pFirst) != lanParty.end()
            && lanParty[pFirst].find(pSecond) != lanParty[pFirst].end();
    }
};

void part1(data);
void part2(data);

int main()
{
    data input;

    std::regex regex("(([^-]+)\\-([^-]+))");
    std::smatch match;
    
    std::string line;
    while (std::getline(fin, line)) {
        if (!std::regex_match(line, match, regex)) {
            exit(1);
        }

        std::string first = match[2];
        std::string second = match[3];
        input.add_connection(first, second);
        input.add_connection(second, first);
    }

    auto part1_start = std::chrono::high_resolution_clock::now();

    part1(input);

    auto part1_end = std::chrono::high_resolution_clock::now();
    auto part1_delta = std::chrono::duration_cast<std::chrono::milliseconds>(part1_end - part1_start);

    std::cout << "\nExecuting part 1 took: " << part1_delta.count() << " ms.\n";

    // This should be about the same value as part1_end, but whatever... the symmetry's nice
    auto part2_start = std::chrono::high_resolution_clock::now();

    part2(input);

    auto part2_end = std::chrono::high_resolution_clock::now();
    auto part2_delta = std::chrono::duration_cast<std::chrono::milliseconds>(part2_end - part2_start);

    std::cout << "\nExecuting part 2 took: " << part2_delta.count() << " ms.\n";
}

void part1(data pInput) {
    num result = 0;

    for (const auto &computer_a : pInput.computers) {
        for (const auto &computer_b : pInput.lanParty[computer_a]) {
            for (const auto &computer_c : pInput.lanParty[computer_b]) {

                // Filter out tuples without the Historian's computer
                if (computer_a.identifier[0] != 't'
                    && computer_b.identifier[0] != 't'
                    && computer_c.identifier[0] != 't') {
                    continue;
                }

                if (pInput.connected(computer_a, computer_c)) {
                    result++;
                }
            }
        }
    }

    // We end up counting permutations too,
    //  so divide by 3!
    result /= 1 * 2 * 3;

    std::cout << "part 1: " << result;
}

void part2(data pInput) {
    // dummy

    std::cout << "part 2: ";
}