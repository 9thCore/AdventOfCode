#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <queue>
std::ifstream fin("input.txt");

typedef long long num;

struct point {
    num i = 0, j = 0;

    bool operator==(const point &pRHS) const {
        return i == pRHS.i && j == pRHS.j;
    }
};

const point DELTA[] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };
const int EAST = 1;

struct cell {
    bool wall;
    bool visited;
};

typedef std::vector<std::vector<cell>> map;

struct data {
    map mapData;
    point start, end;
};

void part1(data);
void part2(data);

int main()
{
    data input;
    
    std::string line;
    int i = 0;
    while (std::getline(fin, line)) {
        int j = 0;
        input.mapData.emplace_back();

        for (const char ch : line) {
            switch (ch) {
            case 'S':
                input.start = { i, j };
                break;
            case 'E':
                input.end = { i, j };
                break;
            }
            input.mapData.back().push_back({ ch == '#', false });
            j++;
        }

        i++;
    }

    part1(input);
    std::cout << "\n";
    part2(input);
}

num bfs(data &pInput) {
    num bestScore = LLONG_MAX;

    struct node {
        bool sameDir;
        int score;
        int dir;
        point pos;

        // no clue why this specifically works :)
        bool operator<(const node &pRHS) const {
            return score > pRHS.score;
        }
    };

    std::priority_queue<node> queue;
    queue.push({ false, 0, EAST, pInput.start });

    while (!queue.empty()) {
        node data = queue.top();
        queue.pop();

        if (pInput.end == data.pos) {
            if (data.score < bestScore) {
                bestScore = data.score;
            }
            continue;
        }

        pInput.mapData[data.pos.i][data.pos.j].visited = 1;

        for (int dir = 0; dir < 4; dir++) {
            point next = { data.pos.i + DELTA[dir].i, data.pos.j + DELTA[dir].j };

            cell &c = pInput.mapData[next.i][next.j];
            if (!c.wall && !c.visited && std::abs(dir - data.dir) != 2) {
                bool turn = dir != data.dir;
                queue.push({ !!turn, data.score + (turn ? 1000 : 0) + 1, dir, next });
            }
        }
    }

    return bestScore;
}

void part1(data pInput) {
    std::cout << "part 1: " << bfs(pInput);
}

void part2(data pInput) {
    // dummy
}