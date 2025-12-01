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
typedef unsigned long long u_num;

struct data {
    enum class operation {
        AND,
        OR,
        XOR
    };

    struct cell {
        enum class value {
            UNINITIALISED,
            ON,
            OFF
        } data = value::UNINITIALISED, pre_process_data = value::UNINITIALISED;

        const std::string identifier;

        cell(const std::string &pIdentifier) : identifier(pIdentifier) { }

        bool initialised() const {
            return data != value::UNINITIALISED;
        }

        bool initialise(bool enabled = false) {
            if (initialised()) {
                return false;
            }

            data = get_initialised(enabled);
            return true;
        }

        void initialise_pre_process(bool enabled = false) {
            pre_process_data = get_initialised(enabled);
        }

        bool enabled() const {
            return data == value::ON;
        }

        bool is_output(u_num &pIndex) const {
            if (identifier[0] != 'z') {
                return false;
            }

            pIndex = std::stoull(identifier.substr(1));
            return true;
        }

        static value get_initialised(bool enabled) {
            return enabled ? value::ON : value::OFF;
        }
    };

    struct expression {
        const cell &operand_a, &operand_b;
        cell &result;
        operation op;

        bool apply() {
            if (!operand_a.initialised()
                || !operand_b.initialised()) {
                return false;
            }

            bool value = 0;
            bool enabled_a = operand_a.enabled(),
                enabled_b = operand_b.enabled();

            switch (op) {
            case operation::AND:
                value = enabled_a && enabled_b;
                break;
            case operation::OR:
                value = enabled_a || enabled_b;
                break;
            case operation::XOR:
                value = enabled_a != enabled_b;
                break;
            }

            return result.initialise(value);
        }
    };

    std::vector<cell> cells;
    std::vector<expression> expressions;

    cell &get_cell(std::string pIdentifier) {
        for (auto &cell : cells) {
            if (cell.identifier == pIdentifier) {
                return cell;
            }
        }

        cells.emplace_back(pIdentifier);
        return cells.back();
    }

    expression &register_expression(const cell &pOperandA, const operation &pOperation, const cell &pOperandB, cell &pResult) {
        expressions.push_back({
            pOperandA,
            pOperandB,
            pResult,
            pOperation
            });
        return expressions.back();
    }

    operation get_op(const std::string &pName) const {
        if (pName == "AND") {
            return operation::AND;
        }
        else if (pName == "OR") {
            return operation::OR;
        }
        else if (pName == "XOR") {
            return operation::XOR;
        }
        
        exit(1);
    }

    void reset() {
        for (auto &cell : cells) {
            cell.data = cell.pre_process_data;
        }
    }
};

void part1(data &);
void part2(data &);

int main()
{
    data input;
    input.cells.reserve(1000);
    input.expressions.reserve(1000);

    std::regex regexes[] = {
        std::regex("^([^:]+): ([01])$"),
        std::regex("^([a-z0-9]{3}) (AND|OR|XOR) ([a-z0-9]{3}) -> ([a-z0-9]{3})$")
    };
    std::smatch match;
    
    std::string line;
    while (std::getline(fin, line)) {
        if (std::regex_match(line, match, regexes[0])) {
            std::string key = match[1];
            std::string value = match[2];
            bool flag = (value[0] == '1');

            data::cell &cell = input.get_cell(key);
            cell.initialise(flag);
            cell.initialise_pre_process(flag);
        }
        else if (std::regex_match(line, match, regexes[1])) {
            std::string operand_a = match[1],
                operation = match[2],
                operand_b = match[3],
                result = match[4];

            data::cell &cell_operand_a = input.get_cell(operand_a),
                &cell_operand_b = input.get_cell(operand_b),
                &cell_result = input.get_cell(result);
            data::operation op = input.get_op(operation);

            input.register_expression(cell_operand_a, op, cell_operand_b, cell_result).apply();
        }
        else if (!line.empty()) {
            exit(1);
        }
    }

    auto part1_start = std::chrono::high_resolution_clock::now();

    part1(input);

    auto part1_end = std::chrono::high_resolution_clock::now();
    auto part1_delta = std::chrono::duration_cast<std::chrono::milliseconds>(part1_end - part1_start);

    std::cout << "\nExecuting part 1 took: " << part1_delta.count() << " ms.\n";

    input.reset();

    // This should be about the same value as part1_end, but whatever... the symmetry's nice
    auto part2_start = std::chrono::high_resolution_clock::now();

    part2(input);

    auto part2_end = std::chrono::high_resolution_clock::now();
    auto part2_delta = std::chrono::duration_cast<std::chrono::milliseconds>(part2_end - part2_start);

    std::cout << "\nExecuting part 2 took: " << part2_delta.count() << " ms.\n";
}

void part1(data &pInput) {
    do {
        bool any_changes_this_loop = false;

        for (auto &expression : pInput.expressions) {
            if (expression.apply()) {
                any_changes_this_loop = true;
            }
        }

        if (!any_changes_this_loop) {
            break;
        }
    } while (true);

    u_num result = 0;

    for (const auto &cell : pInput.cells) {
        u_num index;
        if (cell.enabled() && cell.is_output(index)) {
            result |= (u_num)1 << index;
        }
    }

    std::cout << "part 1: " << result;
}

void part2(data &pInput) {
    // dummy

    std::cout << "part 2: ";
}