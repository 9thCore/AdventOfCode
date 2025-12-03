#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ARBITRARY_INPUT_MAX_LENGTH 250
#define ARBITRARY_BANK_MAX_LENGTH 150
#define ARBITRARY_JOLTAGES_MAX_LENGTH 20

typedef struct {
    char bank[ARBITRARY_BANK_MAX_LENGTH];
} program_input;

typedef struct {
    char joltages[ARBITRARY_JOLTAGES_MAX_LENGTH];
    int joltage_indices[ARBITRARY_JOLTAGES_MAX_LENGTH];
    int joltage_count;
} joltage_holder;

void part1(program_input *, size_t);
void part2(program_input *, size_t);

int main(int argc, char **argv)
{
    program_input input[ARBITRARY_INPUT_MAX_LENGTH];
    size_t input_length = 0;

    while (scanf("%s\n", input[input_length].bank) != EOF) {
        input_length++;
    }

    part1(input, input_length);
    part2(input, input_length);

    return EXIT_SUCCESS;
}

unsigned long long construct_number(joltage_holder *holder) {
    unsigned long long result = 0;
    
    for (int k = 0; k < holder->joltage_count; k++) {
        result *= 10;
        result += holder->joltages[k] - '0';
    }

    return result;
}

void find_best_joltages(joltage_holder *holder, char *bank) {
    size_t bank_length = strlen(bank);

    for (int i = 0; i < holder->joltage_count; i++) {
        holder->joltages[i] = bank[i];
        holder->joltage_indices[i] = i;
    }

    for (int bank_idx = 1; bank_idx < bank_length; bank_idx++) {
        char this_battery = bank[bank_idx];
        size_t reduced_length = bank_length - bank_idx;

        int joltage_idx = holder->joltage_count - reduced_length;
        if (joltage_idx < 0) {
            joltage_idx = 0;
        }

        while (joltage_idx < holder->joltage_count) {

            if (this_battery > holder->joltages[joltage_idx]
            && bank_idx > holder->joltage_indices[joltage_idx]) {
                for (int i = 0; i < holder->joltage_count - joltage_idx; i++) {
                    holder->joltages[joltage_idx + i] = bank[bank_idx + i];
                    holder->joltage_indices[joltage_idx + i] = bank_idx + i;
                }
                break;
            }

            joltage_idx++;
        }
    }
}

void part1(program_input *input, size_t input_length) {
    joltage_holder holder = {{0}, {0}, 2};
    int sum = 0;

    for (size_t i = 0; i < input_length; i++) {
        char *bank = input[i].bank;
        find_best_joltages(&holder, bank);
        sum += construct_number(&holder);
    }

    printf("The answer for part 1 is: %d\n", sum);
}

void part2(program_input *input, size_t input_length) {
    joltage_holder holder = {{0}, {0}, 12};
    unsigned long long sum = 0;

    for (size_t i = 0; i < input_length; i++) {
        char *bank = input[i].bank;
        find_best_joltages(&holder, bank);
        sum += construct_number(&holder);
    }

    printf("The answer for part 2 is: %llu\n", sum);
}