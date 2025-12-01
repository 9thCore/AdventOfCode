#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <deque>
#include <array>
#include <chrono>
std::ifstream fin("input.txt");

typedef long long num;
typedef std::vector<num> data;
typedef std::deque<num> window;
typedef uint32_t window_hash;

constexpr num MAGIC_NUMBER = 16777216 - 1;

void part1(data);
void part2(data);

int main()
{
    data input;
    
    std::string line;
    while (std::getline(fin, line)) {
        input.push_back(std::stoll(line));
    }

    auto start = std::chrono::high_resolution_clock::now();

    part1(input);
    std::cout << "\n";
    part2(input);

    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << "\nTotal time: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << " milliseconds.";
}

void mix(num &pSecretNumber, num pValue) {
    pSecretNumber ^= pValue;
}

void prune(num &pSecretNumber) {
    pSecretNumber &= MAGIC_NUMBER;
}

void mix_and_prune(num &pSecretNumber, num pFactor, bool pDivide = false) {
    num value = pDivide ? (pSecretNumber >> pFactor) : (pSecretNumber << pFactor);
    prune(pSecretNumber);
    mix(pSecretNumber, value);
    prune(pSecretNumber);
}

void evolve_secret(num &pSecretNumber) {
    mix_and_prune(pSecretNumber, 6);
    mix_and_prune(pSecretNumber, 5, true);
    mix_and_prune(pSecretNumber, 11);
}

void part1(data pInput) {
    for (int step = 0; step < 2000; step++) {
        for (num &secretNumber : pInput) {
            evolve_secret(secretNumber);
        }
    }

    num sum = 0;
    for (num secretNumber : pInput) {
        sum += secretNumber;
    }

    std::cout << "part 1: " << sum;
}

num calculate_change(num pLast, num pCurrent) {
    return pCurrent % 10 - pLast % 10;
}

window_hash get_value(const window &pWindow, int pIndex) {
    // Assume the change will always be
    //  in the range [-9, 9], so
    //  pack it into 5 bits:
    //  0: 00000, +1: 00001, ...
    // +9: 01001, -1: 01010,
    // -2: 01011, ...
    // -6: 01111, -7: 10000,
    // -8: 10001, -9: 10010
    num val = pWindow[pIndex];
    if (val < 0) {
        return 9 - val;
    }
    return val;
}

// 20 bit number that represents
//  a given sequence, uniquely.
window_hash hash_window(const window &pWindow) {
    return get_value(pWindow, 0)
        | (get_value(pWindow, 1) << 5)
        | (get_value(pWindow, 2) << 10)
        | (get_value(pWindow, 3) << 15);
}

// Maps a sequence of changes to
//  the final sum, from which we grab
//  the highest value.
std::array<uint16_t, 0xFFFFF + 1> sequence_map = { 0 };

// "Set" of already added hashes for
//  a given buyer, using bitwise
//  operations to pack 32 flags
//  into one 32-wide unsigned integer.
std::array<uint32_t, 0x7FFF + 1> added_map = { 0 };

void part2(data pInput) {
    constexpr int SUCCESSIVE_CHANGE_COUNT = 4;

    // Amortises the cost of finding
    //  the best sequence a bit.
    window_hash biggest_global_hash = 0;

    int idx = 0;
    for (num &secretNumber : pInput) {
        // A sliding window of the current sequence
        //  of changes, using a deque to leverage
        //  the fast popping from the front.
        window sliding_window;

        // Amortises the cost of resetting
        //  the added map a bit.
        window_hash biggest_hash = 0;

        // Prepare the change sliding windows
        //  by simulating the first changes
        for (int step = 0; step < SUCCESSIVE_CHANGE_COUNT; step++) {
            num last = secretNumber;
            evolve_secret(secretNumber);
            sliding_window.push_back(calculate_change(last, secretNumber));
        }

        // Slide window along and map values accordingly
        for (int step = SUCCESSIVE_CHANGE_COUNT; step < 2000; step++) {
            window_hash hash = hash_window(sliding_window);

            window_hash key = hash / 32;
            window_hash bit = 1 << (hash % 32);

            // Check if the current hash has already been added,
            //  and if not, we add it
            if ((added_map[key] & bit) == 0) {
                sequence_map[hash] += secretNumber % 10;
                added_map[key] |= bit;
            }

            biggest_hash = std::max(biggest_hash, hash);
            num last = secretNumber;
            evolve_secret(secretNumber);

            sliding_window.pop_front();
            sliding_window.push_back(calculate_change(last, secretNumber));
        }

        window_hash key = biggest_hash / 32;
        for (window_hash i = 0; i < key; i++) {
            added_map[i] = 0;
        }

        biggest_global_hash = std::max(biggest_global_hash, biggest_hash);
    }

    uint16_t result = 0;
    for (window_hash i = 0; i < biggest_global_hash; i++) {
        result = std::max(sequence_map[i], result);
    }

    std::cout << "part 2: " << result;
}