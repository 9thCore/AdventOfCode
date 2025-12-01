#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <queue>
#include <array>
#include <unordered_map>
#include <functional>
std::ifstream fin("input.txt");

typedef std::vector<std::string> keypad;
typedef std::vector<std::vector<uint64_t>> num_keypad;

// # represents wall
const keypad NUMERIC_KEYPAD = {
    "#####",
    "#789#",
    "#456#",
    "#123#",
    "##0A#",
    "#####"
};

constexpr int NUMERIC_KEYPAD_HEIGHT = 6;
constexpr int NUMERIC_KEYPAD_WIDTH = 5;

const keypad DIRECTIONAL_KEYPAD = {
    "#####",
    "##^A#",
    "#<v>#",
    "#####"
};

constexpr int DIRECTIONAL_KEYPAD_HEIGHT = 4;
constexpr int DIRECTIONAL_KEYPAD_WIDTH = 5;

typedef long long num;

struct point {
    num i = 0, j = 0;

    bool operator==(const point &pRHS) const {
        return i == pRHS.i && j == pRHS.j;
    }

    struct hash {
        std::size_t operator()(const point &pPoint) const {
            return pPoint.i * NUMERIC_KEYPAD_WIDTH + pPoint.j;
        }
    };
};

const point DELTA[] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };
const char MOVE[] = { '^', '>', 'v', '<' };

typedef long long num;

struct data {
    typedef std::string code;
    std::vector<code> codes;
};

constexpr num MAX_SIZE = 'z';

typedef std::array<std::array<std::string, MAX_SIZE>, MAX_SIZE> numeric_cache;
numeric_cache precomputed_cache;
numeric_cache precomputed_cache_part2;

void part1(data);
void part2(data);

int main()
{
    data input;
    
    std::string line;
    while (std::getline(fin, line)) {
        input.codes.emplace_back(line);
    }

    part1(input);
    std::cout << "\n";
    part2(input);
}

bool find_in_keypad(const keypad &pKeys, const char pTargetCharacter, point &pPosition) {
    for (num i = 1; i < pKeys.size() - 1; i++) {
        for (num j = 1; j < pKeys[i].size() - 1; j++) {
            if (pKeys[i][j] == pTargetCharacter) {
                pPosition = { i, j };
                return true;
            }
        }
    }

    return false;
}

void bfs(const keypad &pKeys, const point &pStart, const point &pTarget, std::function<void(const std::string &, const point &)> pCallback) {
    struct node {
        const std::string path;
        point pos;
        num dist, idx;
    };

    if (pStart == pTarget) {
        pCallback("A", pStart);
        return;
    }

    num global_idx = 1;
    std::array<std::array<uint64_t, 10>, 10> visited = { {0} };

    num maxDist = std::abs(pStart.i - pTarget.i) + std::abs(pStart.j - pTarget.j);

    std::queue<node> queue;
    queue.push({ "", pStart, 0, global_idx++ });
    visited[pStart.i][pStart.j] = 1;

    while (!queue.empty()) {
        node data = queue.front();
        queue.pop();

        if (data.dist > maxDist) {
            continue;
        }

        if (data.pos == pTarget) {
            pCallback(data.path + "A", data.pos);
            continue;
        }

        for (int dir = 0; dir < 4; dir++) {
            point next = { data.pos.i + DELTA[dir].i, data.pos.j + DELTA[dir].j };
            if (pKeys[next.i][next.j] == '#' || (visited[next.i][next.j] & data.idx)) {
                continue;
            }

            for (auto &v : visited) {
                for (auto &c : v) {
                    if (c & data.idx) {
                        c |= global_idx;
                    }
                }
            }
            queue.push({ data.path + MOVE[dir], next, data.dist + 1, global_idx++ });
        }
    }
}

void bfs(const keypad &pKeys, const char pStartCharacter, const char pTargetCharacter, std::function<void(const std::string &, const point &)> pCallback) {
    point start, end;
    if (!find_in_keypad(pKeys, pStartCharacter, start)
        || !find_in_keypad(pKeys, pTargetCharacter, end)) {
        return;
    }

    bfs(pKeys, start, end, pCallback);
}

num score_path(const std::string &pPath) {
    num score = 0;
    for (num i = 1; i < pPath.size(); i++) {
        score += (pPath[i] != pPath[i - 1]);
    }
    return score;
}

void iterate_and_get_best_path(const std::string &pPath, const std::function<void(const std::string &, const num)> &pCallback) {
    for (num i = 0; i < pPath.size(); i++) {
        char previous = (i == 0) ? 'A' : pPath[i - 1];
        char current = pPath[i];

        num best_score = LLONG_MAX;
        std::string best_path;

        bfs(DIRECTIONAL_KEYPAD, previous, current, [&](const std::string &pNewPath, const point &pPosition) {
            num score = score_path(pNewPath);
            if (score < best_score) {
                best_score = score;
                best_path = pNewPath;
            }
            });

        if (best_score == LLONG_MAX) {
            continue;
        }

        pCallback(best_path, best_score);
    }
}

void preprocess_recursive(std::string &pResult, const std::string &pPath, int pDepthRemaining) {
    iterate_and_get_best_path(pPath, [&](const std::string &pBest, num) {
        if (pDepthRemaining == 0) {
            pResult += pBest;
        }
        else {
            preprocess_recursive(pResult, pBest, pDepthRemaining - 1);
        }
        });
}

void preprocess(numeric_cache &pCache, int pDepth) {
    // Crunch the numbers and calculate the best human move
    //  from each cell to another on the numeric keypad.
    
    const std::string keys = "A0123456789";

    for (const char start : keys) {
        for (const char end : keys) {
            bfs(NUMERIC_KEYPAD, start, end, [&](const std::string &pPath, const point &pPosition) {
                std::string result = "";
                preprocess_recursive(result, pPath, pDepth);

                std::string &previous = pCache[start][end];
                if (previous.empty() || previous.length() > result.length()) {
                    previous = result;
                }

                });
        }
    }
}

std::string resolve_line(const data &pInput, const numeric_cache &pCache, const data::code &pCode) {
    std::string result = pCache['A'][pCode[0]];
    for (num i = 1; i < pCode.size(); i++) {
        result += pCache[pCode[i - 1]][pCode[i]];
    }
    return result;
}

void part1(data pInput) {
    num result = 0;

    preprocess(precomputed_cache, 1);

    for (const auto &code : pInput.codes) {
        num value = std::stoll(code);
        std::string resolved = resolve_line(pInput, precomputed_cache, code);
        num length = resolved.length();
        result += length * value;
    }

    std::cout << "part 1: " << result;
}

void part2(data pInput) {
    num result = 0;

    preprocess(precomputed_cache_part2, 24);

    for (const auto &code : pInput.codes) {
        num value = std::stoll(code);
        std::string resolved = resolve_line(pInput, precomputed_cache_part2, code);
        num length = resolved.length();
        result += length * value;
    }

    std::cout << "part 2: " << result;
}